package iti.tugraz.at.vis.particle;

import iti.tugraz.at.vis.particle.event.SignalChangedReceiver;

import java.util.ArrayList;
import java.util.List;

public class LightBulb implements Wire
{

    private WireSignal currentSignal = WireSignal.HIGH_Z;
    private List<SignalChangedReceiver> signalReceiver = new ArrayList<>();


    @Override
    public void put(WireSignal signal)
    {
        if (currentSignal == WireSignal.HIGH_Z
                || currentSignal == WireSignal.HIGH)
        {
            currentSignal = signal;

        } else if (currentSignal == WireSignal.LOW)
        {
            if (signal == WireSignal.HIGH)
            {
                currentSignal = WireSignal.HIGH;
                notifyReceiver();
            } else
            {
                currentSignal = signal;
            }
        }
    }


    public boolean isGlowing()
    {
        return currentSignal == WireSignal.HIGH;
    }


    @Override
    public WireSignal get()
    {
        return currentSignal;
    }


    synchronized private void notifyReceiver()
    {
        for (SignalChangedReceiver receiver : signalReceiver)
        {
            receiver.signalChanged();
        }
    }


    @Override
    synchronized public void setSignalChangedReceiver(
            SignalChangedReceiver receiver)
    {
        signalReceiver.add(receiver);
    }


    @Override
    synchronized public void removeSignalChangedReceiver(
            SignalChangedReceiver receiver)
    {
        signalReceiver.remove(receiver);
    }

}
