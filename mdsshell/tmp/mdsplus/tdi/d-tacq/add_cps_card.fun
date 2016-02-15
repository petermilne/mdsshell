fun public add_cps_card(in _node, in _nchan)
{
	Tcl("set def \\TOP");
	_card="."//_node;
	Tcl("add node "//_card);
	Tcl("set def "//_card);
	Tcl("add node/usage=text :_comment");
	Tcl("put :_comment \"\"\"canonical card added by add_card.fun\"\"\" ");
	Tcl("add node/usage=numeric :_DT");
	Tcl("put :_DT 1");
	Tcl("add node/usage=text :_DT:comment");
	Tcl("put :_DT:comment \"\"\"sample interval [sec]
		Overwrite default 1 when using external clock\"\"\" "); 
	Tcl("add node/usage=numeric :_ASHOT");
	Tcl("add node/usage=text :_ASHOT:comment");
	Tcl("put :_ASHOT:comment \"\"\"shot number from target\"\"\" ");

	for(_i=1; _i<= _nchan; _i++)
	{
		_field=":CH"//iii(_i,2);
		Tcl("add node/usage=signal "//_field);
		Tcl("set node/nocompress_on_put "//_field);
	}
	Tcl("add node/usage=numeric ESW01");
	Tcl("set def \\TOP");
}
