#ifndef REPLXX_HISTORY_HXX_INCLUDED
#define REPLXX_HISTORY_HXX_INCLUDED 1

#include <vector>

#include "unicodestring.hxx"
#include "utf8string.hxx"
#include "conversion.hxx"

namespace replxx {

class History {
public:
	typedef std::vector<UnicodeString> entries_t;
private:
	entries_t _entries;
	int _maxSize;
	int _index;
	int _yankIndex;
	/*
	 * _previousIndex and _recallMostRecent are used to allow
	 * HISTORY_NEXT action (a down-arrow key) to have a special meaning
	 * if invoked after a line from history was accepted without
	 * any modification.
	 * Special meaning is: a down arrow shall jump to the line one
	 * after previously accepted from history.
	 */
	int _previousIndex;
	bool _recallMostRecent;
	bool _unique;
public:
	History( void );
	void add( UnicodeString const& line );
	void save( std::string const& filename );
	void load( std::string const& filename );
	void clear( void );
	void set_max_size( int len );
	void set_unique( bool unique_ ) {
		_unique = unique_;
	}
	void reset_pos( int = -1 );
	void reset_yank_iterator();
	bool next_yank_position( void );
	UnicodeString const& operator[] ( int ) const;
	void reset_recall_most_recent( void ) {
		_recallMostRecent = false;
	}
	void drop_last( void ) {
		_entries.pop_back();
	}
	void commit_index( void ) {
		_previousIndex = _index;
		_recallMostRecent = true;
	}
	int current_pos( void ) const {
		return ( _index );
	}
	bool is_last( void ) const {
		return ( _index == ( size() - 1 ) );
	}
	bool is_empty( void ) const {
		return ( _entries.empty() );
	}
	void update_last( UnicodeString const& line_ ) {
		_entries.back() = line_;
	}
	bool move( bool );
	UnicodeString const& current( void ) const {
		return ( _entries[_index] );
	}
	UnicodeString const& yank_line( void ) const {
		return ( _entries[_yankIndex] );
	}
	void jump( bool );
	bool common_prefix_search( UnicodeString const&, int, bool );
	int size( void ) const {
		return ( static_cast<int>( _entries.size() ) );
	}
	Replxx::HistoryScan::impl_t scan( Utf8String& ) const;
private:
	History( History const& ) = delete;
	History& operator = ( History const& ) = delete;
};

class Replxx::HistoryScanImpl {
	History::entries_t const& _entries;
	History::entries_t::const_iterator _it;
	Utf8String& _utf8Cache;
	mutable Replxx::HistoryEntry _entryCache;
public:
	HistoryScanImpl( History::entries_t const&, Utf8String& );
	bool next( void );
	Replxx::HistoryEntry const& get( void ) const;
};

}

#endif

