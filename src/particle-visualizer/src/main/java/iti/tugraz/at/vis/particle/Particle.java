package iti.tugraz.at.vis.particle;

import iti.tugraz.at.vis.particle.event.SignalChangedReceiver;

import java.io.File;
import java.util.logging.Logger;

public class Particle implements Runnable
{
    private class WireASignalChangedReceiver implements SignalChangedReceiver
    {

        @Override
        public void signalChanged()
        {
            Particle.this.signalAChanged();
        }

    }

    private class WireBSignalChangedReceiver implements SignalChangedReceiver
    {

        @Override
        public void signalChanged()
        {
            Particle.this.signalBChanged();
        }

    }

    Logger logger = Logger.getLogger(Particle.class.getCanonicalName());

    private File outPipe;
    private File inPipe;
    private Wire txA;
    private Wire rxA;
    private Wire txB;
    private Wire rxB;
    private int particleId = -1;

    private Boolean isRunning = true;

    private SignalChangedReceiver signalAListener = new WireASignalChangedReceiver();
    private SignalChangedReceiver siglalBListener = new WireBSignalChangedReceiver();


    public Particle(int id, Wire txA, Wire rxA, Wire txB, Wire rxB,
            File inPipe, File outPipe)
    {
        this.txA = txA;
        this.txB = txB;
        this.rxA = rxA;
        this.rxB = rxB;
        this.inPipe = inPipe;
        this.outPipe = outPipe;
    }


    @Override
    public void run()
    {
        rxA.setSignalChangedReceiver(signalAListener);
        rxB.setSignalChangedReceiver(siglalBListener);

        synchronized (isRunning)
        {
            logger.info("[" + particleId + "] tick");
            try
            {
                Thread.sleep(1000);
            }
            catch (InterruptedException e)
            {
            }
        }

        rxA.setSignalChangedReceiver(null);
        rxB.setSignalChangedReceiver(null);
        logger.info("[" + particleId + "] done");
    }


    public void shutdown()
    {
        synchronized (isRunning)
        {
            isRunning = false;
        }
    }


    synchronized private void signalAChanged()
    {

    }


    synchronized private void signalBChanged()
    {

    }
}
