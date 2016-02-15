public fun RFXPoloidal__add(in _path, out _nidout)
{
    DevAddStart(_path, 'RFXPoloidal', 22, _nidout);
    DevAddNode(_path // ':COMMENT', 'TEXT', *, *, _nid);
    DevAddNode(_path // ':LOAD_INDUCT', 'NUMERIC', *, *, _nid);
    DevAddNode(_path // ':LOAD_RESIST', 'NUMERIC', *, *, _nid);
    DevAddNode(_path // ':PLASMA_IND', 'NUMERIC', *, *, _nid);
    DevAddNode(_path // ':PM_UNITS', 'TEXT', *, *, _nid);
    DevAddNode(_path // ':PM_CONFIG', 'TEXT', *, *, _nid);
    DevAddNode(_path // ':PM_CONTROL', 'TEXT', *, *, _nid);
    DevAddNode(_path // ':PM_ENABLED', 'NUMERIC', *, *, _nid);
    DevAddNode(_path // ':PM_WINDOW', 'NUMERIC', *, *, _nid);
    DevAddNode(_path // ':PC_UNITS', 'TEXT', *, *, _nid);
    DevAddNode(_path // ':PC_CONFIG', 'TEXT', *, *, _nid);
    DevAddNode(_path // ':PC_CONTROL', 'TEXT', *, *, _nid);
    DevAddNode(_path // ':PC_ENABLED', 'NUMERIC', *, *, _nid);
    DevAddNode(_path // ':PC_WINDOW', 'NUMERIC', *, *, _nid);
    DevAddNode(_path // ':PV_UNITS', 'TEXT', *, *, _nid);
    DevAddNode(_path // ':PV_CONFIG', 'TEXT', *, *, _nid);
    DevAddNode(_path // ':PV_CONTROL', 'TEXT', *, *, _nid);
    DevAddNode(_path // ':PV_ENABLED', 'NUMERIC', *, *, _nid);
    DevAddNode(_path // ':PV_WINDOW', 'NUMERIC', *, *, _nid);
    DevAddNode(_path // ':R_TRANSFER', 'NUMERIC', *, *, _nid);
    DevAddNode(_path // ':PTCB_VOLTAGE', 'NUMERIC', *, *, _nid);
    DevAddEnd();
}

 