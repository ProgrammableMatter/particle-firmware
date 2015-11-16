package iti.tugraz.at.vis.particle;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import iti.tugraz.at.vis.particle.event.SignalChangedReceiver;

import org.junit.Before;
import org.junit.Test;

public class LightBulbTest implements SignalChangedReceiver
{
    private int signalChangedCount = 0;


    @Before
    public void resetSignalCounter()
    {
        signalChangedCount = 0;
    }


    @Test
    public void lightBulbTest_testPassingSignal_beginningWithHigh()
    {
        LightBulb a = new LightBulb();
        a.setSignalChangedReceiver(this);
        // wire is high_z
        a.put(WireSignal.HIGH);
        assertEquals(WireSignal.HIGH, a.get());
        assertTrue(a.isGlowing());
        assertEquals(0, signalChangedCount);

        a.put(WireSignal.LOW);
        assertEquals(WireSignal.LOW, a.get());
        assertFalse(a.isGlowing());
        assertEquals(0, signalChangedCount);

        a.put(WireSignal.HIGH);
        assertEquals(WireSignal.HIGH, a.get());
        assertTrue(a.isGlowing());
        assertEquals(1, signalChangedCount);

        a.removeSignalChangedReceiver(this);
    }


    @Test
    public void lightBulbTest_testPassingSignal_beginningWithLow()
    {
        LightBulb a = new LightBulb();
        a.setSignalChangedReceiver(this);
        // wire is high_z
        a.put(WireSignal.LOW);
        assertEquals(WireSignal.LOW, a.get());
        assertFalse(a.isGlowing());
        assertEquals(0, signalChangedCount);

        a.put(WireSignal.HIGH);
        assertEquals(WireSignal.HIGH, a.get());
        assertTrue(a.isGlowing());
        assertEquals(1, signalChangedCount);

        a.put(WireSignal.LOW);
        assertEquals(WireSignal.LOW, a.get());
        assertFalse(a.isGlowing());

        a.removeSignalChangedReceiver(this);
    }


    @Override
    public void signalChanged()
    {
        signalChangedCount++;
    }
}
