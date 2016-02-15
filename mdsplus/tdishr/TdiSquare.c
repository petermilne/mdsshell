/*	Tdi3Square.C
	Interludes for some simple stuff.

	Ken Klare, LANL CTR-7	(c)1989,1990
*/
#include <mdsdescrip.h>
#include <STATICdef.h>

STATIC_CONSTANT char *cvsrev = "@(#)$RCSfile: TdiSquare.c,v $ $Revision: 1.3 $ $Date: 2003/11/17 21:21:21 $";

extern int Tdi3Multiply();
extern int Tdi3Complex();
extern int Tdi3UnaryMinus();
extern int Tdi3Floor();

/*--------------------------------------------------------------
	Square a number by multiplying.
*/
int			Tdi3Square(
struct descriptor	*in_ptr,
struct descriptor	*out_ptr)
{
	return Tdi3Multiply(in_ptr, in_ptr, out_ptr);
}
/*--------------------------------------------------------------
	F90 elemental, convert complex call to molded type.
*/
int			Tdi3Cmplx(
struct descriptor	*x_ptr,
struct descriptor	*y_ptr,
struct descriptor	*mold_ptr,
struct descriptor	*out_ptr)
{
        struct descriptor *dummy = mold_ptr;
	return Tdi3Complex(x_ptr, y_ptr, out_ptr);
}
/*--------------------------------------------------------------
	F90 elemental, round to higher integral value. In CC it is CEIL.
*/
int			Tdi3Ceiling(
struct descriptor	*in_ptr,
struct descriptor	*out_ptr)
{
int	status;

	status = Tdi3UnaryMinus(in_ptr, out_ptr);
	if (status & 1) status = Tdi3Floor(out_ptr, out_ptr);
	if (status & 1) status = Tdi3UnaryMinus(out_ptr, out_ptr);
	return status;
}
