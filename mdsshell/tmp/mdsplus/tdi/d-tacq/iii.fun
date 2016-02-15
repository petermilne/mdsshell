/*
 iii(3) gives the string '003', iii(3,5) gives '00003'
 code by XL @ TCV
*/

fun public iii(in _j, optional in _n)
{
        Return(translate(text(_j,present(_n) ? _n : 3),"0"," "));
}
