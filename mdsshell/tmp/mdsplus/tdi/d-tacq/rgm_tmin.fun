/*
	rgm_tmin(_nbefore) : timebas label for pulse [nbefore end] pulse, in minutes
	round to %60s to avoid nasty timebase plot alias effect
*/

fun public rgm_tmin(optional in _nbefore)
{
	_chx = 'CH00';
	_seg =execute('GetNumSegments('//_chx//');');
	_seg -= present(_nbefore)? _nbefore : 1;
 	_ans = execute('dim_of(GetSegment('//_chx//','//_seg//'))[1]/60;');	
	return(_ans);
}
