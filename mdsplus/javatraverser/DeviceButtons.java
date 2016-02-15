import javax.swing.*;

public class DeviceButtons extends JPanel
{
    protected DeviceControl apply, ok;
    public String methods[];
    
    public void setMethods(String methods[])
    {
        this.methods = methods;
    }
    
    public String [] getMethods() { return methods; }
   
    public DeviceButtons()
    {
        add(ok = new DeviceOk());
        add(apply = new DeviceApply());
        add(new DeviceReset());
        add(new DeviceCancel());
    }
    public void setCheckExpressions(String []checkExpressions)
    {
        apply.setCheckExpressions(checkExpressions);
        ok.setCheckExpressions(checkExpressions);
    }
    public String [] getCheckExpressions()
    {
        return apply.getCheckExpressions();
    }
    
    public void setCheckMessages(String []checkMessages)
    {
        apply.setCheckMessages(checkMessages);
        ok.setCheckMessages(checkMessages);
    }
    public String [] getCheckMessages()
    {
        return apply.getCheckMessages();
    }
    
        
}

       