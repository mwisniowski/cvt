#ifndef CVT_LIST_H
#define CVT_LIST_H

#include <stdint.h>
#include <string.h>

namespace cvt {

	template<typename T>
	class List {
		private:
			struct NodeBase {
				NodeBase( NodeBase* next = NULL , NodeBase* prev = NULL ) : _next( next ), _prev( prev ) {}
				NodeBase( const NodeBase& it ) : _next( it._next ), _prev( it._prev )  {}
				NodeBase& operator=( const NodeBase& it ) { if( &it != this ) { _next = it._next; _prev = it._prev; } return *this; }

				NodeBase* _next;
				NodeBase* _prev;
			};

			struct Node : public NodeBase {
				Node( const T& data, NodeBase* next = NULL, NodeBase* prev = NULL ) : NodeBase( next, prev ), _data( data ) {}
				Node( const Node& node ) : NodeBase( node._next, node._prev ), _data( node._data ) {}
				Node& operator=( const Node& it ) { if( &it != this ) { NodeBase::_next = it._next; NodeBase::_prev = it._prev; _data = it._data; } return *this; }

				T _data;
			};


		public:
			List();
			~List();
			List( const List<T>& list );
			List& operator=( const List<T>& list );

			void append( const T& data );
			void prepend( const T& data );
			void clear();
			size_t size() const;
			bool isEmpty() const;

			class Iterator {
				friend class List;
				public:
					Iterator() : _it( NULL ) {}
					Iterator( const Iterator& it ) : _it( it._it ) {}
					Iterator& operator=( const Iterator& it ) { _it = it._it; return *this; }

					bool operator==( const Iterator& it ) { return _it == it._it; }
					bool operator!=( const Iterator& it ) { return _it != it._it; }

					Iterator& operator++() { _it = _it->_next; return *this; }
					Iterator operator++( int ) { Iterator ret( *this ); _it = _it->_next; return ret; }
					Iterator& operator--() { _it = _it->_prev; return *this; }
					Iterator operator--( int ) { Iterator ret( *this ); _it = _it->_prev; return ret; }

					T& operator*() { return ( ( Node* ) _it )->_data; }

				private:
					Iterator( NodeBase* it ) : _it( it ) {}
					NodeBase* _it;
			};

			Iterator begin() { return Iterator( _anchor._prev ); }
			Iterator end() { return Iterator( &_anchor ); }

			class ReverseIterator {
				friend class List;
				public:
					ReverseIterator() : _it( NULL ) {}
					ReverseIterator( const ReverseIterator& it ) : _it( it._it ) {}
					ReverseIterator& operator=( const ReverseIterator& it ) { _it = it._it; return *this; }

					bool operator==( const ReverseIterator& it ) { return _it == it._it; }
					bool operator!=( const ReverseIterator& it ) { return _it != it._it; }

					ReverseIterator& operator++() { _it = _it->_prev; return *this; }
					ReverseIterator operator++( int ) { ReverseIterator ret( *this ); _it = _it->_prev; return ret; }
					ReverseIterator& operator--() { _it = _it->_next; return *this; }
					ReverseIterator operator--( int ) { ReverseIterator ret( *this ); _it = _it->_next; return ret; }

					T& operator*() { return ( ( Node* ) _it )->_data; }

				private:
					ReverseIterator( NodeBase* it ) : _it( it ) {}
					NodeBase* _it;
			};

			Iterator rbegin() { return Iterator( _anchor._next ); }
			Iterator rend() { return Iterator( &_anchor ); }

		private:
			NodeBase  _anchor;
			size_t	  _size;
	};

	template<typename T>
	inline List<T>::List() : _anchor( &_anchor, &_anchor ), _size( 0 )
	{
	}

	template<typename T>
	inline List<T>::~List()
	{
		clear();
	}

	template<typename T>
	inline List<T>::List( const List<T>& list )
	{
		Node* it = list._anchor._next;
		while( it ) {
			append( it->_data );
			it = it->_next;
		}
	}

	template<typename T>
	inline size_t List<T>::size() const
	{
		return _size;
	}

	template<typename T>
	inline bool List<T>::isEmpty() const
	{
		return _size == 0;
	}

	template<typename T>
	inline List<T>& List<T>::operator=( const List<T>& list )
	{
		if( this != &list ) {
			clear();
			NodeBase* it = list._anchor->next;
			while( it ) {
				append( it->_data );
				it = it->_next;
			}
		}
		return *this;
	}

	template<typename T>
	inline void List<T>::append( const T& data )
	{
		if( _anchor._next == &_anchor ) {
			Node* node = new Node( data, &_anchor, &_anchor );
			_anchor._next = node;
			_anchor._prev = node;
		} else {
			Node* node = new Node( data, &_anchor, _anchor._next );
			_anchor._next->_next = node;
			_anchor._next = node;
		}
		_size++;
	}

	template<typename T>
	inline void List<T>::prepend( const T& data )
	{
		if( _anchor._prev == &_anchor ) {
			Node* node = new Node( data, &_anchor, &_anchor );
			_anchor._next = node;
			_anchor._prev = node;
		} else {
			Node* node = new Node( data, _anchor._prev, &_anchor );
			_anchor._prev->_prev = node;
			_anchor._prev = node;
		}
		_size++;
	}

	template<typename T>
	inline void List<T>::clear()
	{
		if( isEmpty() )
			return;

		Iterator it = begin();
		Iterator iend = end();
		while( it != iend ) {
			Iterator cur = it;
			it++;
			delete ( Node* ) cur._it;
		}
		_anchor._prev = &_anchor;
		_anchor._next = &_anchor;
		_size = 0;
	}
}

#endif
