#include <string>
#include <cstring>    // for strchr


#include <iostream>

using namespace std;


#define TOKCHARS \
"ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
"abcdefghijklmnopqrstuvwxyz" \
"01234567890" \
"_$:%"

/** Parser: split string into keywords/not-keywords. store both. */
template <typename Container> class Parser{
/** tokenizes input string taking quotes into account. */
	int in_quotes;
	char const * const tokchars;
	char const quote;

	Container tokens;
	string s0;

	bool istok (char c)
	{
#if 0
		if (c == quote){
			in_quotes = !in_quotes;
			return 0;
		}else if (in_quotes){
			return 0;
		}else{
			return (strchr(tokchars,c) != NULL);
		}
#else
		return (strchr(tokchars,c) != NULL);
#endif
	}
	void create(string const &s) {	      
		const string::size_type  S = s.size();
		string::size_type  left = 0;
		string::size_type  right = 0;

		for (int in_token = 0; left < S; 
		     left = right++, in_token = !in_token ) {
			while ((right < S) && (in_token == istok(s[right]))){
				++right;
			}

			if (right > left){
				tokens.push_back(s.substr(left, right-left));
			}
		}
	}
public:
        Parser(string const &s, 
		  char const * const _tokchars = TOKCHARS,
		  char const _quote = '\'') : 
		in_quotes(0), tokchars(_tokchars), quote(_quote), s0(s)
	{
		create(s0);
	}

	Parser(const Parser<Container>& source) :
		tokchars(source.tokchars), quote(source.quote), s0(source.s0)
	{
		create(s0);
	}

public:
	Container& list() {
		return tokens;
	}
	const Container& const_list() const {
		return tokens;
	}
};

