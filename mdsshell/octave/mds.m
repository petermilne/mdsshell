# interface to mdsPlus thin client

mds_debug = 0;

function _system(cmd)
	global mds_debug;

	if (mds_debug > 0)
		printf("system %s\n", cmd);
	endif
	system(cmd);
endfunction

function mdsConnect(server)
	cmd = sprintf("mdsConnect %s", server);
	_system(cmd);
endfunction

function mdsOpen(tree)
	cmd = sprintf("mdsOpen %s", tree);
	_system(cmd);
endfunction

function mdsClose(tree)
	cmd = sprintf("mdsClose %s", tree);
	_system(cmd);
endfunction

function mdsValue(expr)
	cmd = sprintf("mdsValue %s", expr);
	_system(cmd);
endfunction


function tempname = mktemp(template)
	if (nargin < 1)
		template = "/tmp/mdsshell.XXXXXX";
	endif
	cmd = sprintf("mktemp %s", template);
	[tempname, status] = system(cmd);
	tempname(length(tempname)) = ''; 	
endfunction

function mdsPut(expr, x, precision)
	if (nargin < 3)
		precision = "short";
	endif
	tmpname = mktemp();
	fp = fopen(tmpname, "w");
	fwrite(fp, x, precision);
	fclose(fp);
	cmd = sprintf("mdsPut --precision %s --dim %d --file %s %s",
			precision, length(x), tmpname, expr);
	_system(cmd);
endfunction

function x = mdsGet(expr, precision)
	if (nargin < 2)
		precision = "short";
	endif
	tmpname = mktemp();
	x = 1:1024;
	cmd = sprintf("mdsValue --file %s %s", tmpname, expr);
	fp = fopen(tmpname, "r");
	x = fread(fp, Inf, precision);
	printf( "read from %s npoints %d\n", tmpname, length(x));
	fclose(fp);
endfunction


