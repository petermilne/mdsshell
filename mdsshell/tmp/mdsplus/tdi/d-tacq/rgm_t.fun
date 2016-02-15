/*
	rgm_t(_nbefore) : timebase for pulse [nbefore end] pulse, in sec
	round to %60s to avoid nasty timebase plot alias effect
*/

fun public rgm_t(optional in _nbefore)
{
	_chx='CH00';
	_seg=execute('GetNumSegments('//_chx//');');
	_seg -= present(_nbefore)? _nbefore : 1;
 	_ans = execute('GetSegment('//_chx//','//_seg//') MOD 60;');
	return(_ans);
}
