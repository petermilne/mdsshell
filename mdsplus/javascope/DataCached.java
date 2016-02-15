/* $Id: DataCached.java,v 1.20 2003/08/08 12:35:34 manduchi Exp $ */
import java.io.Serializable;

public class DataCached implements Serializable
 {
    String expression;
    String experiment;
    long    shot;
    Object data;
    
    DataCached(String expression, String experiment, long shot, Object data)
    {
        this.expression = expression;
        this.shot = shot;
        this.experiment = experiment;
        this.data = data;
    }
    
    public boolean equals(String expression, String experiment, long shot)
    {
        return ( this.expression.equals(expression) && 
                 this.shot == shot && 
                 this.experiment.equals(experiment));
    }
 }
