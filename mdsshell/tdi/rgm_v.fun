/*
	rgm_v(ch) : output the last pulse [nbefore end] pulse, in volts
*/

fun public rgm_v(in _ch, optional in _nbefore)
{
	_chx='CH'//iii(_ch, 2);
	_gain=_chx//'.GAIN_V';
	_off=_chx//'.OFFSET_V';
	_seg=execute('GetNumSegments('//_chx//');');
	_seg -= present(_nbefore)? _nbefore : 1;
 	_ans = execute('GetSegment('//_chx//','//_seg//')*'//_gain//'+'//_off//';');
	return(_ans);
}
