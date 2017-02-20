#include<string.h>
#include<stdio.h>
#include <options/Option.h>



int optionSet(Option *ctx,const char * key, const char * val, int search_flags){
	const Instruction *ins = ctx->ops;
	if (ins == NULL){
		return -1;
	}
	void *dst = ((uint8_t *)ctx) + ins->offset;
	while (ins->name != NULL){
		if (!strcmp(ins->name, key)){
			switch (ins->type){
			case OPT_TYPE_INT:
				{
					int num;
					if (sscanf_s(val, "%d", &num) == 1){
						*(int *)dst = num;
					}
				 }
				break;
			case OPT_TYPE_INT64:
				{
					int64_t num;
					if (sscanf_s(val, "%d", &num) == 1){
						*(int64_t *)dst = num;
					}
				}
				break;
			case OPT_TYPE_STRING:
				{
						*(char **)dst = _strdup(val);
				}
				break;
				default:
					break;
			}
		}
		ins++;
	}
}
