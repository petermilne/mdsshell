/* $Id: LocalDataProvider.java,v 1.26 2004/05/27 11:36:02 manduchi Exp $ */
import javax.swing.JFrame;
import java.io.IOException;
import java.util.*;

public class LocalDataProvider extends MdsDataProvider implements DataProvider
{
    Vector listeners = new Vector();
    Vector eventNames = new Vector();

    static class EventDescriptor
    {
        UpdateEventListener listener;
        String event;
        int evId;
        EventDescriptor(UpdateEventListener listener, String event, int evId)
        {
            this.listener = listener;
            this.event = event;
            this.evId = evId;

        }
        public boolean equals(Object obj)
        {
            if(!(obj instanceof EventDescriptor)) return false;
            EventDescriptor evDescr = (EventDescriptor)obj;
            return listener == evDescr.getListener() && event.equals(evDescr.getEvent());
        }
        UpdateEventListener getListener() {return listener;}
        String getEvent() {return event;}
        int getEvId() {return evId;}
    }
    static {
        try
        {
	        System.loadLibrary("JavaMds");
	    }
	    catch(Throwable e)
	    {
	    }
    }

    native public void SetEnvironmentSpecific(String in, String defaultNode);
    native public void Update(String exp, long s);
    native public String GetString(String in);
    native public float GetFloat(String in);

    public synchronized float[] GetFloatArray(String in)  throws IOException
    {
      //  in = "( _jscope_"+var_idx+" = ("+in+"), fs_float(_jscope_"+var_idx+"))";// "fs_float(("+in+"))";
        var_idx++;
        return GetFloatArrayNative(in);
    }
    public synchronized double[] GetDoubleArray(String in)  throws IOException
    {
      //  in = "( _jscope_"+var_idx+" = ("+in+"), ft_float(_jscope_"+var_idx+"))";// "fs_float(("+in+"))";
        var_idx++;
        return GetDoubleArrayNative(in);
    }
    public synchronized RealArray GetRealArray(String in)  throws IOException
    {
      return new RealArray(GetDoubleArray(in));
    }


    native public float[] GetFloatArrayNative(String in);
    native public double[] GetDoubleArrayNative(String in);
    native public int[]   GetIntArray(String in);
    native public long[]  GetLongArray(String in);

    public long[] GetShots(String in)
    {
        try {
            int shots[] =  GetIntArray(in.trim());
            long lshots[] = new long[shots.length];
            for(int i = 0; i < shots.length; i++)
                lshots[i] = shots[i];
            return lshots;
        }catch(Exception exc)
        {
            System.err.println("Error in GetLongArray: " + exc);

            return null;
        }
    }

    native public byte [] GetByteArray(String in);
    native public String ErrorString();
    public void AddUpdateEventListener(UpdateEventListener l, String event)
    {
        int evId;
        int idx;
        try {
            evId = getEventId(event);
            idx = eventNames.indexOf(event);
        }catch(Exception exc)
        {
            idx = eventNames.size();
            eventNames.addElement(event);
            evId = registerEvent(event, idx);
        }
        listeners.addElement(new EventDescriptor(l, event, evId));
    }
    public void RemoveUpdateEventListener(UpdateEventListener l, String event)
    {
        int idx = listeners.indexOf(new EventDescriptor(l, event, 0));
        if(idx != -1)
        {
            int evId = ((EventDescriptor)listeners.elementAt(idx)).getEvId();
            listeners.removeElementAt(idx);
            try {
                int id = getEventId(event);
            }catch(Exception exc)
            {
                unregisterEvent(evId);
            }
        }

    }
    public void AddConnectionListener(ConnectionListener l){}

    public void RemoveConnectionListener(ConnectionListener l){}
//    public float[]  GetFrameTimes(String in_frame) {return null; }
//    public byte[] GetFrameAt(String in_frame, int frame_idx) {return null; }

    public FrameData GetFrameData(String in_y, String in_x, float time_min, float time_max) throws IOException
    {
        throw(new IOException("Frames visualization on LocalDataProvider not implemented"));
    }

    protected synchronized boolean  CheckOpen() {return true; }
    public boolean SupportsCompression(){return false;}
    public void SetCompression(boolean state){}
    public boolean SupportsContinuous() {return false; }
    public boolean DataPending() {return  false;}
    public int     InquireCredentials(JFrame f, DataServerItem server_item){return DataProvider.LOGIN_OK;}
    public boolean SupportsFastNetwork(){return false;}
    public void    SetArgument(String arg){};
    public boolean SupportsTunneling(){return false;}


    int getEventId(String event) throws Exception
    {
        for(int idx = 0; idx < listeners.size(); idx++)
        {
            EventDescriptor evDescr = (EventDescriptor)listeners.elementAt(idx);
            if(event.equals(evDescr.getEvent()))
                return evDescr.getEvId();
        }
        throw(new Exception());
    }

    public void fireEvent(int nameIdx)
    {
        String event = (String)eventNames.elementAt(nameIdx);
        for(int idx = 0; idx < listeners.size(); idx++)
        {
            EventDescriptor evDescr = (EventDescriptor)listeners.elementAt(idx);
            if(evDescr.getEvent().equals(event))
                evDescr.getListener().processUpdateEvent(new UpdateEvent(this, event));
        }
    }

    native public int registerEvent(String event, int idx);
    native public void unregisterEvent(int evId);
}
