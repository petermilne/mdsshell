/* $Id: WaveformListener.java,v 1.20 2003/08/08 12:35:42 manduchi Exp $ */
import java.util.EventListener;

public interface WaveformListener extends EventListener 
{
    public void processWaveformEvent(WaveformEvent e);
}
