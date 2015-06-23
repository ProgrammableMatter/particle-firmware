package iti.tugraz.at.vis.particle;

import iti.tugraz.at.vis.particle.event.SignalChangedReceiver;

public interface Wire
{
    /**
     * write state to wire
     * 
     * @param s
     */
    public void put(WireSignal s);


    /**
     * read current state from wire
     * 
     * @return
     */
    public WireSignal get();
    
    /**
     * register new signal changed receiver
     * @param receiver
     */
    public void setSignalChangedReceiver(SignalChangedReceiver receiver);

    
    /**
     * remove signal changed receiver
     * @param receiver
     */
    public void removeSignalChangedReceiver(SignalChangedReceiver receiver);

}
