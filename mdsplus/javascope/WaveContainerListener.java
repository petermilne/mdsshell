/* $Id: WaveContainerListener.java,v 1.20 2003/08/08 12:35:41 manduchi Exp $ */
import java.util.EventListener;

public interface WaveContainerListener extends EventListener 
{
    public void processWaveContainerEvent(WaveContainerEvent e);
}
