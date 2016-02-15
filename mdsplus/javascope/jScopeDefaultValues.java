/* $Id: jScopeDefaultValues.java,v 1.22 2003/08/08 12:35:42 manduchi Exp $ */
class jScopeDefaultValues 
{
   long	  shots[];
   String xmin, xmax, ymax, ymin;
   String title_str, xlabel, ylabel;
   String experiment_str, shot_str;
   String upd_event_str, def_node_str;
   boolean is_evaluated = false;
   String public_variables = null;
   boolean upd_limits = true;

   public void Reset()
   {
        shots = null;
        xmin = xmax = ymax = ymin = null;
        title_str = xlabel = ylabel = null;
        experiment_str = shot_str = null;
        upd_event_str = def_node_str = null;
        is_evaluated = false;
        upd_limits = true;
   }
}
