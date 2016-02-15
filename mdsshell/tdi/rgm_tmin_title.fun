/*
	rgm_tmin(_nbefore) : timebas label for pulse [nbefore end] pulse, in minutes
	round to %60s to avoid nasty timebase plot alias effect
*/

fun public rgm_tmin_title(in _acq, in _ch)
{
	_tmin = rgm_tmin(_ch); 
	_ans = _acq // '.CH' // iii(_ch,2) // ' Volts    at ' //
		int(_tmin,10) // ' minutes';
	return(_ans);
}
