import java.util.*;

class Balancer implements ServerListener
/**
Ensures action dispatching to servers, keeping load balancing.
*/
{
    Hashtable servers = new Hashtable();
    /**
    indexed by server class. Stores the vector of servers associated with the server class.
    */
    Server default_server = null;


    public synchronized void setDefaultServer(Server server)
    {
        default_server = server;
    }

    public synchronized void addServer(Server server)
    {
        Vector server_vect = (Vector)servers.get(server.getServerClass());
        if(server_vect == null)
        {
            server_vect = new Vector();
            servers.put(server.getServerClass(), server_vect);
        }
        server_vect.addElement(server);
        server.addServerListener(this);
    }
    public void enqueueAction(Action action)
    {
        String server_class;
        try{
            server_class = ((DispatchData)action.getAction().getDispatch()).getIdent().getString();
        }
        catch(Exception exc)
        {
            if(default_server != null)
                default_server.pushAction(action);
            return;
        }


  /*      Vector server_vect = (Vector)servers.get(server_class);
        if(server_vect == null)
        {
            if(default_server != null)
                default_server.pushAction(action);
            return;
        }*/
        Vector server_vect = new Vector();
        Vector all_server_vect = (Vector)servers.get(server_class);
         if(all_server_vect == null)
         {
             if(default_server != null)
                 default_server.pushAction(action);
             return;
         }
         else
          {
            for (int i = 0; i < all_server_vect.size(); i++) {
              Server curr_server = (Server) all_server_vect.elementAt(i);
              if (curr_server.isReady())
                server_vect.addElement(all_server_vect.elementAt(i));
            }
          }
          if (server_vect.size() == 0) {
            if (default_server != null)
              default_server.pushAction(action);
            return;
          }
     /////////////////////////////

        Enumeration server_list = server_vect.elements();
        int curr_load, min_load = 1000000;
        Server curr_server, min_server = null;
        while(server_list.hasMoreElements())
        {
            curr_server = (Server)server_list.nextElement();
            curr_load = curr_server.getQueueLength();
            if(curr_load == 0 && curr_server.isActive())
            {
                curr_server.pushAction(action);
                return;
            }
            if(curr_load < min_load && curr_server.isActive())
            {
                min_load = curr_load;
                min_server = curr_server;
            }
        }
        if(min_server != null)
            min_server.pushAction(action);
        else if(default_server != null)//try to send to default server
            default_server.pushAction(action);
        else
        //all servers inactive (no mdsip connection): send action to either server, it will be aborted
            ((ActionServer)server_vect.elementAt(0)).pushAction(action);
    }

    public void actionStarting(ServerEvent event){}
    public void actionAborted(ServerEvent event){}
    public synchronized void actionFinished(ServerEvent event)
    {
        String server;
        try{
            server = ((DispatchData)event.getAction().getAction().getDispatch()).getIdent().getString();
        }catch(Exception exc) {return; }
        Vector server_vect = (Vector)servers.get(server);
        if(server_vect == null) //it is the default server
            return;
        if(!isBalanced(server_vect))
        {
            Server min_loaded = null, max_loaded = null;
            int max_load = 0;
            Enumeration server_list = server_vect.elements();
            while (server_list.hasMoreElements())
            {
                Server curr_server = (Server)server_list.nextElement();
                int curr_len = curr_server.getQueueLength();
                if(curr_len == 0)
                    min_loaded = curr_server;
                if(curr_len > max_load)
                {
                    max_load = curr_len;
                    max_loaded = curr_server;
                }
            }
            Action action = max_loaded.popAction();
            if(action != null)
                min_loaded.pushAction(action);
        }
    }

    protected boolean isBalanced(Vector server_vect)
    {
        if(server_vect.size() <= 1) return true; //No load balancing if one server in the server class
        Enumeration server_list = server_vect.elements();
        int min_load = 1000000, max_load = 0;
        while(server_list.hasMoreElements())
        {
            Server curr_server = (Server)server_list.nextElement();
            if(!curr_server.isActive()) continue;
            int curr_load = curr_server.getQueueLength();
            if(curr_load < min_load) min_load = curr_load;
            if(curr_load > max_load) max_load = curr_load;
        }
        return !(min_load == 0 && max_load > 1);
    }

    public void abort()
    {
        Enumeration server_vects = servers.elements();
        while(server_vects.hasMoreElements())
        {
            Vector server_vect = (Vector)server_vects.nextElement();
            Enumeration server_list = server_vect.elements();
            while(server_list.hasMoreElements())
            {
                Server curr_server = (Server)server_list.nextElement();
                curr_server.abort(true);
            }
        }
    }

   public void abortAction(Action action)
   {
        Enumeration server_vects = servers.elements();
        while(server_vects.hasMoreElements())
        {
            Vector server_vect = (Vector)server_vects.nextElement();
            Enumeration server_list = server_vect.elements();
            while(server_list.hasMoreElements())
            {
                Server curr_server = (Server)server_list.nextElement();
                if(curr_server.abortAction(action))
                    return;
            }
        }
    }

}
