
#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include <stdlib.h>

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

static char *enum_lits[] = 
{
  TYPEVALS
};

#define ENUM_SIZE (sizeof(enum_lits)/sizeof(char*))

typedef struct enum_order
{
	int order;
	char *value;
} enum_search;

static enum_search enum_table[ENUM_SIZE];
static bool init_done = false;

static int enum_table_cmp(const void * left, const void * right)
{
	enum_search * l = (enum_search *)left;
	enum_search * r = (enum_search *)right;
	return(pg_strcasecmp(l->value, r->value));
}

static void enum_init(void)
{
	int i;

	init_done = true;

	for (i=0; i < ENUM_SIZE ; i++)
	{
		enum_table[i].order = i;
		enum_table[i].value = enum_lits[i];
	}
	qsort(enum_table, ENUM_SIZE, sizeof(enum_search), enum_table_cmp);
	
}

PG_FUNCTION_INFO_V1(TYPENAME_in);
PG_FUNCTION_INFO_V1(TYPENAME_out);

Datum           TYPENAME_in(PG_FUNCTION_ARGS);
Datum           TYPENAME_out(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(TYPENAME_lt);
PG_FUNCTION_INFO_V1(TYPENAME_le);
PG_FUNCTION_INFO_V1(TYPENAME_eq);
PG_FUNCTION_INFO_V1(TYPENAME_ne);
PG_FUNCTION_INFO_V1(TYPENAME_ge);
PG_FUNCTION_INFO_V1(TYPENAME_gt);
PG_FUNCTION_INFO_V1(TYPENAME_cmp);

Datum           TYPENAME_lt(PG_FUNCTION_ARGS);
Datum           TYPENAME_le(PG_FUNCTION_ARGS);
Datum           TYPENAME_eq(PG_FUNCTION_ARGS);
Datum           TYPENAME_ne(PG_FUNCTION_ARGS);
Datum           TYPENAME_ge(PG_FUNCTION_ARGS);
Datum           TYPENAME_gt(PG_FUNCTION_ARGS);
Datum           TYPENAME_cmp(PG_FUNCTION_ARGS);


PG_FUNCTION_INFO_V1(TYPENAME_text);
PG_FUNCTION_INFO_V1(text_TYPENAME);
PG_FUNCTION_INFO_V1(TYPENAME_order);

Datum           TYPENAME_text(PG_FUNCTION_ARGS);
Datum           text_TYPENAME(PG_FUNCTION_ARGS);
Datum           TYPENAME_order(PG_FUNCTION_ARGS);


static int16 lookup(char * str)
{
    int16      result = -1;
	enum_search lookup;
	enum_search * found;

	if (! init_done)
		enum_init();

	/* should probably be optimised by implementing bsearch inline */
	lookup.value=str;
	found = bsearch(&lookup,enum_table,ENUM_SIZE,
					sizeof(enum_search),enum_table_cmp);

	if (found != NULL)
		result = found->order;

	return result;

}



Datum
TYPENAME_in(PG_FUNCTION_ARGS)
{
    char       *str = PG_GETARG_CSTRING(0);
	int16       result;

	result = lookup(str);

    if (result == -1)
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("invalid input value for enum: \"%s\"",
                        str)));

    PG_RETURN_INT16(result);
}

Datum
TYPENAME_out(PG_FUNCTION_ARGS)
{
    int16  internal_val = (int16) PG_GETARG_INT16(0);

	char  *result = pstrdup(enum_lits[internal_val]);

    PG_RETURN_CSTRING(result);
}



Datum           
TYPENAME_lt(PG_FUNCTION_ARGS)
{
	int16 left  = (int16) PG_GETARG_INT16(0);
	int16 right = (int16) PG_GETARG_INT16(1);
	PG_RETURN_BOOL(left < right);
}

Datum           
TYPENAME_le(PG_FUNCTION_ARGS)
{
	int16 left  = (int16) PG_GETARG_INT16(0);
	int16 right = (int16) PG_GETARG_INT16(1);
	PG_RETURN_BOOL(left <= right);
}

Datum           
TYPENAME_eq(PG_FUNCTION_ARGS)
{
	int16 left  = (int16) PG_GETARG_INT16(0);
	int16 right = (int16) PG_GETARG_INT16(1);
	PG_RETURN_BOOL(left == right);
}

Datum           
TYPENAME_ne(PG_FUNCTION_ARGS)
{
	int16 left  = (int16) PG_GETARG_INT16(0);
	int16 right = (int16) PG_GETARG_INT16(1);
	PG_RETURN_BOOL(left != right);
}

Datum           
TYPENAME_ge(PG_FUNCTION_ARGS)
{
	int16 left  = (int16) PG_GETARG_INT16(0);
	int16 right = (int16) PG_GETARG_INT16(1);
	PG_RETURN_BOOL(left >= right);
}

Datum           
TYPENAME_gt(PG_FUNCTION_ARGS)
{
	int16 left  = (int16) PG_GETARG_INT16(0);
	int16 right = (int16) PG_GETARG_INT16(1);
	PG_RETURN_BOOL(left > right);
}

Datum           
TYPENAME_cmp(PG_FUNCTION_ARGS)
{
	int16 left  = (int16) PG_GETARG_INT16(0);
	int16 right = (int16) PG_GETARG_INT16(1);
	PG_RETURN_INT32((int)left - (int)right);
}

Datum
TYPENAME_text(PG_FUNCTION_ARGS)
{
	int16 arg  = (int16) PG_GETARG_INT16(0);
	char * cstr = enum_lits[arg];
	int len = strlen(cstr);
	text *result = (text *) palloc(VARHDRSZ + len);
	VARATT_SIZEP(result) = VARHDRSZ + len;
	memcpy(VARATT_DATA(result), cstr, len);
	PG_RETURN_TEXT_P(result);
}

#define GET_STR(textp) DatumGetCString(DirectFunctionCall1(textout, PointerGetDatum(textp)))

Datum
text_TYPENAME(PG_FUNCTION_ARGS)
{
	text *the_text  = PG_GETARG_TEXT_P(0);
	char * str = GET_STR(the_text);
	int16 result;

	result = lookup(str);

    if (result == -1)
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("invalid input value for enum: \"%s\"",
                        str)));

    PG_RETURN_INT16(result);
}

Datum
TYPENAME_order(PG_FUNCTION_ARGS)
{
	int16 arg  = (int16) PG_GETARG_INT16(0);
	PG_RETURN_INT32((int)arg);
}






