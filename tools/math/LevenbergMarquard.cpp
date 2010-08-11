/*
 *  LevenbergMarquard.cpp
 *  VisionTools
 *
 *  Created by Sebastian Klose on 07.08.10.
 *  Copyright 2010. All rights reserved.
 *
 */

namespace tools {
	
	LevenbergMarquard::LevenbergMarquard(TerminationCriteria termCrit):
	maxIterations(20),
	maxEpsilon(0.0001),
	lamda(0.001),
	lamdaUpdateFactor(10.0),
	terminationCriteria(termCrit)
	{
	}
	
	LevenbergMarquard::~LevenbergMarquard()
	{
	}
	
	void LevenbergMarquard::setMaxIterations(unsigned int maxIters)
	{
		maxIterations = maxIters;
	}
	
	void LevenbergMarquard::setMaxEpsilon(double maxEps)
	{
		maxEpsilon = maxEps;
	}
	
	/*template <class CostFunctionType, class JacobianFunctionType>
	double LevenbergMarquard::optimize(CostFunctionType& costs,
									   JacobianFunctionType& jacFunc,
									   Eigen::VectorXd & startParameters,
									   std::vector<Eigen::VectorXd> & measurements,
									   std::vector<Eigen::VectorXd> & model,
									   Eigen::VectorXd & resultParameters)*/
	double LevenbergMarquard::optimize(CostFunctionType & costFunction,
									   JacobianFunctionType& jacobianFunction,
									   Eigen::VectorXd const& startParameters,
									   std::vector<Eigen::VectorXd> const& measurements,
									   std::vector<Eigen::VectorXd> const& model,
									   Eigen::VectorXd & resultParameters)
	{
		int measurementDimension = measurements[0].rows();
		int jacRows = measurementDimension * (int)measurements.size();
		int parameterDimension = startParameters.rows();
		
		Eigen::VectorXd currentDeltaP(parameterDimension);
		Eigen::VectorXd jTres(parameterDimension);
		
		
		Eigen::MatrixXd jacobian(jacRows, parameterDimension);
		Eigen::VectorXd residual(jacobian.rows());
		Eigen::MatrixXd jT(parameterDimension, jacobian.rows());
		Eigen::MatrixXd jTj(parameterDimension, parameterDimension);
		
		resultParameters = startParameters;
		
		// start the iteration:
		unsigned int currentIter = 1;
		double currentCosts = costFunction(resultParameters, model, measurements, residual);
		double lastCosts;
		
		double decay = 1.0;
		
		while (true) {
			jacobianFunction(resultParameters,
							 model,
							 jacobian);
			
			// get the jacobians
			jT = jacobian.transpose();
			jTj = jT*jacobian;
			jTres = jT*residual;
			jTres*=-1.0;
			
			// add lambda to the diagonal
			jTj.diagonal().cwise()+=lamda;
			
			// save the costs before the update:
			lastCosts = currentCosts;
			
			// do the update:
			jTj.svd().solve(jTres, &currentDeltaP);
			
			// update parameters:
			resultParameters+=currentDeltaP;
			
			// evaluate the current costs
			currentCosts = costFunction(resultParameters, model, measurements, residual);
			
			while(currentCosts > lastCosts){
				//std::cout << "Current lambda:" << lamda << std::endl;
				//std::cout << "Wrong direction" << std::endl;
				
				//  undo changes
				resultParameters-=currentDeltaP;					
				jTj.diagonal().cwise()-=lamda;
				
				lamda *= (lamdaUpdateFactor);
				
				// solve again with new lambda step
				jTj.diagonal().cwise()+=lamda;
				jTj.svd().solve(jTres, &currentDeltaP);
				resultParameters+=currentDeltaP;
				currentCosts = costFunction(resultParameters, model, measurements, residual);
				
				currentIter++;
				if(checkTermination(currentIter, currentCosts))
					break;
			}
			
			lamda/=lamdaUpdateFactor;
			
			currentIter++;
			if(checkTermination(currentIter, currentCosts))
			{
				std::cout << "Numiterations: " << currentIter << std::endl;
				break;
			}
			
			if(lastCosts == currentCosts){
				if (currentDeltaP.squaredNorm() == 0.0) {
					std::cout << "Ran into minimum" << std::endl;
					break;
				}
			}
			
			//std::cout << "CurrentIteration: " << currentIter << " Epsilon: " << currentCosts << std::endl;
			//std::cout << "CurrentDeltaP: " << currentDeltaP << std::endl;
		}
		
		lamda = 0.001;
		
		return currentCosts;
	}
	
	bool LevenbergMarquard::checkTermination(unsigned int currIter, double currEpsilon)
	{
		if(terminationCriteria == Iterations || terminationCriteria == IterationsOrEpsilon){
			if (currIter > maxIterations) {
				return true;
			}
		}
		
		if(terminationCriteria == Epsilon || terminationCriteria == IterationsOrEpsilon){
			if (currEpsilon < maxEpsilon) {
				return true;
			}
		}
		
		return false;
	}
	
}
