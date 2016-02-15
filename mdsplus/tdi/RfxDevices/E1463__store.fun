public fun E1463__store(as_is _nid, optional _method)
{
    private _K_CONG_NODES = 45;
    private _N_ADDRESS = 1;
    private _N_COMMENT = 2;
    private _N_TRIG_MODE = 3;
    private _N_TRIG_SOURCE = 4;
    private _N_SYNCH_MODE = 5;
    private _N_SYNCH_SOURCE = 6;
    private _N_FRAME_1 = 7;
    private _N_EXP_TIME = 8;
    private _N_N_SCANS = 9;
    private _N_HEAD_TEMP = 10;
    private _N_SCAN_MASK = 11;
    private _N_DATA = 12;
    private _N_BACK = 13;

    _address = if_error(data(DevNodeRef(_nid, _N_ADDRESS)),(DevLogErr(_nid, "Missing GPIB Address"); abort();));
    _id = GPIBGetId(_address);
    if(_id == 0)
    {
	DevLogErr(_nid, "Invalid GPIB Address");
	abort();
    }

    _synch_mode = if_error(data(DevNodeRef(_nid, _N_SYNCH_MODE)),(DevLogErr(_nid, "No valid synch mode defined"); abort();));
    if(_synch_mode == 'EXTERNAL')
    {
	_synch_clock = if_error(data(DevNodeRef(_nid, _N_SYNCH_SOURCE)),(DevLogErr(_nid, "No synch source defined"); abort();));    
	_synch_time = if_error(data(slope(_synch_clock)),(DevLogErr(_nid, "No valid synch source defined"); abort();)); 
    }
    else
	_synch_time = if_error(data(DevNodeRef(_nid, _N_EXP_TIME)),(DevLogErr(_nid, "No exposure time defined"); abort();));

    _trig_mode = if_error(data(DevNodeRef(_nid, _N_TRIG_MODE)),(DevLogErr(_nid, "Missing trigger mode"); abort();));
    if(_trig_mode == 'EXTERNAL')
    	_trig_time = if_error(data(DevNodeRef(_nid, _N_TRIG_SOURCE)),(DevLogErr(_nid, "Missing external trigger"); abort();));
    else
	_trig_time = 0;

    _pts = GPIBQuery(_id, '#PTS', 5);
    if(_pts == 0)
    {
	DevLogErr(_nid, "Cannot make GPIB query for PTS");
	abort();
    }
    _n_scans =  if_error(data(DevNodeRef(_nid, _N_N_SCANS)),(DevLogErr(_nid, "Missing number of scans"); abort();));
    _dim1 = make_dim(make_window(0, _n_scans - 1, _trig_time), make_range(*,*,_synch_time));
    _dim2 = make_dim(make_window(0, _pts - 1, 0),  make_range(*,*,1));

    
    _command = 'BDSINT 1,1,' // trim(adjustl(_pts));    
    if_error(GPIBWrite(_id, _command),(DevLogErr(_nid, "Error in GPIB Write"); abort();)); 
    _command = 'BDSINT 1,1,' // trim(adjustl(_pts));    
    _line = if_error(GPIBReadShorts(_id, _pts), (DevLogErr(_nid, "Error in GPIB Read"); abort();)); 
    if_error(GPIBWrite(_id, _command),(DevLogErr(_nid, "Error in GPIB Write"); abort();)); 
    _line = if_error(GPIBReadShorts(_id, _pts), (DevLogErr(_nid, "Error in GPIB Read"); abort();)); 

    _back_nid = DevHead(_nid) + _N_BACK;
    _signal = compile('build_signal((`_line), $VALUE, (`_dim2))');
    TreeShr->TreePutRecord(val(_back_nid),xd(_signal),val(0));
    _lines = [];
    write(*, 'Start readout');
    for(_i = 1; _i < (_n_scans + 1); _i++)
    {
	_command = 'BDSINT ' // trim(adjustl(_i)) // ',1,' // trim(adjustl(_pts));
   	if_error(GPIBWrite(_id, _command),(DevLogErr(_nid, "Error in GPIB Write"); abort();)); 
wait(0.1);
   	_line = if_error(GPIBReadShorts(_id, _pts), (DevLogErr(_nid, "Error in GPIB Read"); abort();));
	_lines = [_lines, _line];
    }
    write(*, 'End readout');
    _data_nid = DevHead(_nid) + _N_DATA;
    _signal = compile('build_signal((`_lines), $VALUE, (`_dim2), (`_dim1))');
    return (TreeShr->TreePutRecord(val(_data_nid),xd(_signal),val(0)));

}

