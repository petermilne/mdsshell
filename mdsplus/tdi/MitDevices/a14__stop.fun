public fun a14__stop(as_is _nid, optional _method)
{
  _name = DevNodeRef(_nid,1);
  return(DevCamChk(_name,CamPiow(_name,1,25,_d=0,24),1,*));  /* Stop */
}
