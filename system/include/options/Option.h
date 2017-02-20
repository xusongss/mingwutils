#ifndef _OPTIONS_H_
#define _OPTIONS_H_
#include<stdint.h>
enum OptionType{
	OPT_TYPE_INT,
	OPT_TYPE_INT64,
	OPT_TYPE_STRING

};
struct Instruction{
	const char *name;

	/**
    * short English help text
    * @todo What about other languages?
    */
	const char *help;
	/**
    * The offset relative to the context structure where the option
    * value is stored. It should be 0 for named constants.
    */
	int offset;
	enum OptionType type;

	/**
    * the default value for scalar options
    */
	/**
 * the default value for scalar options
 */
	union {
		int64_t i64;
		double dbl;
		const char *str;
	} default_val;
	double min;                 ///< minimum valid value for the option
	double max;                 ///< maximum valid value for the option

	int flags;
	const char *unit;
};
struct Option
{
	Instruction *ops;
};
int optionSet(Option *ctx,const char * key, const char * val, int search_flags =0);
#endif

