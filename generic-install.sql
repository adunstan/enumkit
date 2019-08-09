

CREATE FUNCTION TYPENAME_in(cstring)
    RETURNS TYPENAME
    AS 'TYPENAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION TYPENAME_out(TYPENAME)
    RETURNS cstring
    AS 'TYPENAME'
    LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE TYPENAME (
   internallength = 2, 
   input = TYPENAME_in,
   output = TYPENAME_out,
   alignment = int2,
   passedbyvalue
);


CREATE FUNCTION TYPENAME_lt(TYPENAME, TYPENAME)
RETURNS bool
AS 'TYPENAME'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION TYPENAME_le(TYPENAME, TYPENAME)
RETURNS bool
AS 'TYPENAME'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION TYPENAME_eq(TYPENAME, TYPENAME)
RETURNS bool
AS 'TYPENAME'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION TYPENAME_ge(TYPENAME, TYPENAME)
RETURNS bool
AS 'TYPENAME'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION TYPENAME_gt(TYPENAME, TYPENAME)
RETURNS bool
AS 'TYPENAME'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION TYPENAME_ne(TYPENAME, TYPENAME)
RETURNS bool
AS 'TYPENAME'
LANGUAGE C STRICT IMMUTABLE;


CREATE OPERATOR < (
        LEFTARG = TYPENAME,
        RIGHTARG = TYPENAME,
--      NEGATOR = >=,
        PROCEDURE = TYPENAME_lt
);

CREATE OPERATOR <= (
        LEFTARG = TYPENAME,
        RIGHTARG = TYPENAME,
--      NEGATOR = >,
        PROCEDURE = TYPENAME_le
);

CREATE OPERATOR = (
        LEFTARG = TYPENAME,
        RIGHTARG = TYPENAME,
        COMMUTATOR = =,
--      NEGATOR = <>,
        PROCEDURE = TYPENAME_eq
);

CREATE OPERATOR >= (
        LEFTARG = TYPENAME,
        RIGHTARG = TYPENAME,
        NEGATOR = <,
        PROCEDURE = TYPENAME_ge
);

CREATE OPERATOR > (
        LEFTARG = TYPENAME,
        RIGHTARG = TYPENAME,
        NEGATOR = <=,
        PROCEDURE = TYPENAME_gt
);

CREATE OPERATOR <> (
        LEFTARG = TYPENAME,
        RIGHTARG = TYPENAME,
        NEGATOR = =,
        PROCEDURE = TYPENAME_ne
);



CREATE FUNCTION TYPENAME_cmp(TYPENAME, TYPENAME)
RETURNS integer
AS 'TYPENAME'
LANGUAGE c STRICT IMMUTABLE;

-- Create default operator class for 'TYPENAME'    --
-- Needed to create index or primary key       --

CREATE OPERATOR CLASS TYPENAME_ops
DEFAULT FOR TYPE TYPENAME USING btree 
AS
        OPERATOR        1       < ,
        OPERATOR        2       <= ,
        OPERATOR        3       = ,
        OPERATOR        4       >= ,
        OPERATOR        5       > ,
        FUNCTION        1       TYPENAME_cmp(TYPENAME, TYPENAME);


CREATE FUNCTION TYPENAME_order(TYPENAME)
RETURNS integer
AS 'TYPENAME'
LANGUAGE c STRICT IMMUTABLE;

CREATE FUNCTION text(TYPENAME)
RETURNS text
AS 'TYPENAME', 'TYPENAME_text'
LANGUAGE c STRICT IMMUTABLE;


CREATE CAST (TYPENAME AS text)
WITH FUNCTION text(TYPENAME)
AS IMPLICIT;


CREATE FUNCTION TYPENAME(text)
RETURNS TYPENAME
AS 'TYPENAME', 'text_TYPENAME'
LANGUAGE c STRICT IMMUTABLE;


CREATE CAST (text AS TYPENAME)
WITH FUNCTION TYPENAME(text)
AS IMPLICIT;


