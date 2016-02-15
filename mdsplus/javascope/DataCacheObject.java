/* $Id: DataCacheObject.java,v 1.20 2003/08/08 12:35:34 manduchi Exp $ */
import java.io.Serializable;

class DataCacheObject implements Serializable
{
    float data[];
    float x[];
    float y[];
    float up_err[];
    float low_err[];
    int dimension;
    String title;
    String x_label;
    String y_label;
    String z_label;
}
