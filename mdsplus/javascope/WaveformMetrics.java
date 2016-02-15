/* $Id: WaveformMetrics.java,v 1.29 2004/05/27 07:50:44 manduchi Exp $ */
import java.awt.*;
import java.awt.image.*;
import java.awt.event.*;
import java.util.*;
import java.io.Serializable;


public  class WaveformMetrics implements Serializable
{
    static double MAX_VALUE = 10000.;//(double)Integer.MAX_VALUE;
    static double MIN_VALUE = -10000.;//(double)Integer.MIN_VALUE;
    static int INT_MAX_VALUE = (int)MAX_VALUE;
    static int INT_MIN_VALUE = (int)MIN_VALUE;
    double xmax, xmin, ymax, ymin;
    double xrange, //xmax - xmin
	       yrange; //ymax - ymin
    double y_range;
    boolean x_log, y_log;
    double x_offset;
    double x_range;
    int start_x;
    double FACT_X, FACT_Y, OFS_X, OFS_Y;

    int horizontal_offset, vertical_offset;

    static IndexColorModel cm = null;
    static final double LOG10 = 2.302585092994, MIN_LOG = 10E-100;

    public WaveformMetrics(double _xmax, double _xmin,
	double _ymax, double _ymin, Rectangle limits, Dimension d, boolean _x_log, boolean _y_log,
	int horizontal_offset, int vertical_offset)
    {
     	int ylabel_width = limits.width, xlabel_height = limits.height;
	    double delta_x, delta_y;
	    int border_y;

	    this.horizontal_offset = horizontal_offset;
	    this.vertical_offset = vertical_offset;
	    if(_ymin > _ymax) _ymin = _ymax;
	    if(_xmin > _xmax) _xmin = _xmax;
	    start_x = ylabel_width ;
        x_log = _x_log;
	    y_log = _y_log;

	    border_y = xlabel_height;
//    y_range = (d.height - border_y)/(double)d.height;
        y_range = (d.height - border_y - 2*vertical_offset)/(double)d.height;

//	x_range = (d.width - start_x)/(double)d.width;
	    x_range = (d.width - start_x - 2*horizontal_offset)/(double)d.width;
	    x_offset = start_x/(double)d.width;

	    if(x_log)
	    {
	        if(_xmax < MIN_LOG) _xmax = MIN_LOG;
	        if(_xmin < MIN_LOG) _xmin = MIN_LOG;

	        xmax = Math.log(_xmax)/LOG10;
	        xmin = Math.log(_xmin)/LOG10;
	    }
	    else
	    {
	        xmax = _xmax;
	        xmin = _xmin;
	    }
        delta_x = xmax - xmin;
	    xmax += delta_x/100.;
	    xmin -= delta_x /100.;

	    if(y_log)
	    {
	        if(_ymax < MIN_LOG) _ymax = MIN_LOG;
	        if(_ymin < MIN_LOG) _ymin = MIN_LOG;
	        ymax = Math.log(_ymax)/LOG10;
	        ymin = Math.log(_ymin)/LOG10;
	    }
	    else
	    {
	        ymax = _ymax;
	        ymin = _ymin;
	    }
	    delta_y = ymax - ymin;
	    ymax += delta_y/50;
	    ymin -= delta_y / 50.;

	    xrange = xmax - xmin;
	    yrange = ymax - ymin;

	    if(xrange <= 0)
	    {
	        xrange = (double)1E-10;
	        x_offset = 0.5;
	    }
 	    if(yrange <= 0)
	    {
	        yrange = (double)1E-10;
	    }
    }
    final public double XMax() {return xmax;}
    final public double YMax() {return ymax;}
    final public double XMin() {return xmin;}
    final public double YMin() {return ymin;}
    final public double XRange() {return xmax - xmin;}
    final public double YRange() {return ymax - ymin;}
    final public boolean XLog() { return x_log;}
    final public boolean YLog() {return y_log;}
    final public void ComputeFactors(Dimension d)
    {
//	OFS_X = x_offset * d.width - xmin*x_range*d.width/xrange + 0.5;
	OFS_X = x_offset * d.width - xmin*x_range*d.width/xrange + horizontal_offset + 0.5;
	FACT_X = x_range*d.width/xrange;
//	OFS_Y = y_range * ymax*d.height/yrange + 0.5;
	OFS_Y = y_range * ymax*d.height/yrange + vertical_offset + 0.5;
	FACT_Y = -y_range * d.height/yrange;
    }
    final public int XPixel(double x)
    {
	double xpix = x * FACT_X + OFS_X;
	if(xpix >= MAX_VALUE)
	    return INT_MAX_VALUE;
	if(xpix <= MIN_VALUE)
	    return INT_MIN_VALUE;
	return (int)xpix;
    }
    final public int YPixel(double y)
    {
	double ypix = y * FACT_Y + OFS_Y;
	if(ypix >= MAX_VALUE)
	    return INT_MAX_VALUE;
	if(ypix <= MIN_VALUE)
	    return INT_MIN_VALUE;
	return (int)ypix;
    }

    final public int XPixel(double x, Dimension d)
    {
	double ris;
	if(x_log)
	{
	    if( x < MIN_LOG) x = MIN_LOG;
	    x = Math.log(x)/LOG10;
	}
	ris = (x_offset + x_range * (x - xmin)/xrange)*d.width + 0.5;
	if(ris > 20000)
	    ris = 20000;
	if(ris < -20000)
	    ris = -20000;
	return (int)ris;
    }
    final public int YPixel(double y, Dimension d )
    {
	if(y_log)
	{
	    if(y < MIN_LOG) y = MIN_LOG;
	    y = Math.log(y)/LOG10;
	}
	double ris = (y_range * (ymax - y)/yrange)*d.height + 0.5;
	if(ris > 20000)
	    ris = 20000;
	if(ris < -20000)
	    ris = -20000;
	return (int)ris;
    }
    final public double XValue(int x, Dimension d)
    {
	double ris = (double)(((x - 0.5)/d.width - x_offset) * xrange / x_range + xmin);
	if(x_log)
	    return Math.exp(LOG10*ris);
	else
	    return ris;
    }
    final public double YValue(int y, Dimension d)
    {
	double ris = (double)(ymax - ((y - 0.5)/d.height) * yrange/y_range);
	if(y_log)
	    return Math.exp(LOG10*ris);
	else
	    return ris;
    }


    private IndexColorModel getColorModel()
    {
        byte rgb[] = new byte[256], b = 0;
        for(int i = 0; i < 256; i++, b++)
            rgb[i] = b;
        return new IndexColorModel(8, 256, rgb, rgb, rgb);
    }

    private void drawRectagle(Graphics g, int x, int y, int w, int h, int cIdx)
    {
	        g.setColor(new Color(cm.getRed(cIdx), cm.getGreen(cIdx),
	                              cm.getBlue(cIdx), cm.getAlpha(cIdx)));
	        g.fillRect(x, y, w, h);
    }


    public void ToImage(Signal s, Image img, Dimension d)
    {
        int i, j;
        int xSt, xEt, ySt, yEt;
        Graphics2D g2 = (Graphics2D)img.getGraphics();

        if(cm == null)
            cm = getColorModel();

	    ComputeFactors(d);

	    g2.setColor(Color.white);
	    g2.fillRect(0, 0, d.width - 1, d.height - 1);

	    for(i = 1; i < s.time.length && s.time[i] < xmin; i++);
	    xSt = i-1;
	    for(;i < s.time.length-1 && s.time[i] < xmax; i++);
	    xEt = i+1;
	    for(i = 1; i < s.x_data.length && s.x_data[i] < ymin; i++);
	    ySt = i-1;
	    for(;i < s.x_data.length-1 && s.x_data[i] < ymax; i++);
	    yEt = i+1;

	    int p = 0;
	    int h = 0;
	    int w = 0;
	    int yPix0;
	    int yPix1;
	    int xPix0;
	    int xPix1;
	    int pix;

	    yPix1 = (YPixel(s.x_data[ySt+1]) + YPixel(s.x_data[ySt])) / 2;
	    yPix1 = 2*YPixel(s.x_data[ySt]) - yPix1;

	    for(int y = ySt; y < yEt - 1; y++)
	    {
	        yPix0 = yPix1;
	        yPix1 = (YPixel(s.x_data[y+1]) + YPixel(s.x_data[y])) / 2;
	        h  = yPix0 - yPix1;

	        p = y * s.time.length + xSt;

	        xPix1 = (XPixel(s.time[xSt]) + XPixel(s.time[xSt+1]))/2;
	        xPix1 = 2 * XPixel(s.time[xSt]) - xPix1;

	        for(int x = xSt; x < xEt-1 && p < s.data.length; x++)
	        {
	            xPix0 = xPix1;
	            xPix1 = (XPixel(s.time[x+1]) + XPixel(s.time[x]))/2;
	            w = xPix1 - xPix0;

	  	        pix = (int)(255 * (s.data[p++] - s.data_min)/(s.data_max - s.data_min));
	            drawRectagle(g2, xPix0, yPix1, w, h, pix);
	        }

	        if(p < s.data.length)
	        {
	            xPix0 = xPix1;
	            w = 2 * (XPixel(s.time[xEt-1]) - xPix1);
	            pix = (int)(255 * (s.data[p] - s.data_min)/(s.data_max - s.data_min));
	            drawRectagle(g2, xPix0, yPix1, w, h, pix);
	        } else
	            break;
        }


	    yPix0 = yPix1;
	    yPix1 = 2 * YPixel(s.x_data[yEt-1]) - yPix1;
	    h  = yPix0 - yPix1;

	    xPix1 = (XPixel(s.time[xSt]) + XPixel(s.time[xSt+1]))/2;
	    xPix1 = 2 * XPixel(s.time[xSt]) - xPix1;

	    for(int x = xSt; x < xEt-1 && p < s.data.length; x++)
	    {
	        xPix0 = xPix1;
	        xPix1 = (XPixel(s.time[x+1]) + XPixel(s.time[x]))/2;
	        w = xPix1 - xPix0;

	  	    pix = (int)(255 * (s.data[p++] - s.data_min)/(s.data_max - s.data_min));
	        drawRectagle(g2, xPix0, yPix1, w, h, pix);
	    }

	    if(p < s.data.length)
	    {
	        xPix0 = xPix1;
	        w = 2 * (XPixel(s.time[xEt-1]) - xPix1);
	        pix = (int)(255 * (s.data[p] - s.data_min)/(s.data_max - s.data_min));
	        drawRectagle(g2, xPix0, yPix1, w, h, pix);
	    }
    }

    public Vector ToPolygonsDoubleX(Signal sig, Dimension d)
    {
            int i, j, curr_num_points, curr_x, start_x, max_points;
            float  max_y, min_y, curr_y;
            Vector curr_vect = new Vector(5);
            int xpoints[], ypoints[];
            Polygon curr_polygon = null;
            int pol_idx = 0;
            min_y = max_y = sig.y[0];
            xpoints = new int[sig.n_points];
            ypoints = new int[sig.n_points];
            //xpoints = new int[2*sig.n_points];
            //ypoints = new int[2*sig.n_points];
            curr_num_points = 0;
            i = j = 0;
        int end_point = sig.n_points;


            if(x_log || y_log)
            {
                double xmax_nolog = Math.pow(10, xmax);
            double xmin_nolog = Math.pow(10, xmin);

            float first_y, last_y;
                for(i = 0; i < sig.n_points && sig.x_double[i] < xmin_nolog; i++);
                if(i > 0) i--;
                min_y = max_y = sig.y[i];
                j = i+1;
                start_x = XPixel(sig.x_double[i], d);

                first_y = last_y = sig.y[i];
                while(j < end_point)//sig.n_points  && sig.x_double[j] < xmax_nolog)
                {
                        for(j = i+1; j < sig.n_points &&
                            (pol_idx >= sig.n_nans || j != sig.nans[pol_idx])&&
                            (curr_x = XPixel(sig.x_double[j], d)) == start_x; j++)
                        {
                            last_y = curr_y = sig.y[j];
                            if(curr_y < min_y) min_y = curr_y;
                            if(curr_y > max_y) max_y = curr_y;
                        }
                        if(max_y > min_y)
                        {
                            if(first_y != min_y)
                            {
                                xpoints[curr_num_points] = start_x;
                                ypoints[curr_num_points] = YPixel(first_y, d);
                                curr_num_points++;
                            }
                            xpoints[curr_num_points] = xpoints[curr_num_points + 1] = start_x;
                            ypoints[curr_num_points] = YPixel(min_y, d);
                            ypoints[curr_num_points + 1] = YPixel(max_y, d);
                            curr_num_points +=2;
                            if(last_y != max_y)
                            {
                                xpoints[curr_num_points] = start_x;
                                ypoints[curr_num_points] = YPixel(last_y, d);
                                curr_num_points++;
                            }
                        }
                        else
                        {
                            xpoints[curr_num_points] = start_x;
                            ypoints[curr_num_points] = YPixel(max_y, d);
                            curr_num_points++;
                        }
                        if(j == sig.n_points || j == end_point || Float.isNaN(sig.y[j]))// || sig.x_double[j] >= xmax_nolog)
                        {
                            curr_polygon = new Polygon(xpoints, ypoints, curr_num_points);
                            curr_vect.addElement(curr_polygon);
                            pol_idx++;
                            curr_num_points = 0;
                            if(j < sig.n_points)  //need to raise pen
                            {
                                    while(j < sig.n_points && Float.isNaN(sig.y[j]))
                                        j++;
                            }
                        }
                        if(j < end_point)//sig.n_points)
                        {
                            start_x = XPixel(sig.x_double[j], d);
                            max_y = min_y = sig.y[j];
                            i = j;
                            if(sig.x_double[j] > xmax)
                                end_point = j + 1;
                        }
                }
            }
            else // Not using logaritmic scales
            {
                ComputeFactors(d);

                for(i = 0; i < sig.n_points && sig.x_double[i] < xmin; i++);
                if(i > 0) i--;
                min_y = max_y = sig.y[i];
                j = i+1;

        //GAB testare da qua il problema


                start_x = XPixel(sig.x_double[i]);
                float first_y, last_y;
                while(j < end_point)//sig.n_points && sig.x_double[j] < xmax + dt)
                {
                    first_y = last_y = sig.y[i];
                        for(j = i+1; j < sig.n_points && //!Float.isNaN(sig.y[j]) &&
                            (pol_idx >= sig.n_nans || j != sig.nans[pol_idx])&&
                            (curr_x = XPixel(sig.x_double[j])) == start_x; j++)
                        {
                            last_y = curr_y = sig.y[j];
                            if(curr_y < min_y) min_y = curr_y;
                            if(curr_y > max_y) max_y = curr_y;
                        }
                        if(max_y > min_y)
                        {
                            if(first_y == min_y)
                            {
                                xpoints[curr_num_points] = start_x;
                                ypoints[curr_num_points] = YPixel(first_y);
                                curr_num_points++;
                                if(last_y == max_y)
                                {
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(last_y);
                                    curr_num_points++;
                                }
                                else
                                {
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(max_y);
                                    curr_num_points++;
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(last_y);
                                    curr_num_points++;
                                }
                            }
                            else if(first_y == max_y)
                            {
                                xpoints[curr_num_points] = start_x;
                                ypoints[curr_num_points] = YPixel(first_y);
                                curr_num_points++;
                                if(last_y == min_y)
                                {
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(last_y);
                                    curr_num_points++;
                                }
                                else
                                {
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(min_y);
                                    curr_num_points++;
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(last_y);
                                    curr_num_points++;
                                }
                            }
                            else //first_y != min_y && first_y != max_y
                            {
                                xpoints[curr_num_points] = start_x;
                                ypoints[curr_num_points] = YPixel(first_y);
                                curr_num_points++;
                        if(last_y == min_y)
                        {
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(max_y);
                                    curr_num_points++;
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(last_y);
                                    curr_num_points++;
                                }
                                else if(last_y == max_y)
                                {
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(min_y);
                                    curr_num_points++;
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(last_y);
                                    curr_num_points++;
                                }
                                else
                                {
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(min_y);
                                    curr_num_points++;
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(max_y);
                                    curr_num_points++;
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(last_y);
                                    curr_num_points++;
                                }
                            }
                        }
                        else
                        {
                            xpoints[curr_num_points] = start_x;
                            ypoints[curr_num_points] = YPixel(max_y);
                            curr_num_points++;
                        }
                        if(j == sig.n_points || j >= end_point || Float.isNaN(sig.y[j]))// || sig.x_double[j] >= xmax)
                        {
                            curr_polygon = new Polygon(xpoints, ypoints, curr_num_points);
                            curr_vect.addElement(curr_polygon);
                            pol_idx++;
                            curr_num_points = 0;
                            if(j < sig.n_points)  //need to raise pen
                            {
                                    while(j < sig.n_points && Float.isNaN(sig.y[j]))
                                        j++;
                            }
                        }
                        if(j < end_point)//sig.n_points)
                        {
                            start_x = XPixel(sig.x_double[j]);
                            max_y = min_y = sig.y[j];
                            i = j;
                            if(sig.isIncreasingX() && sig.x_double[j] > xmax)
                                end_point = j + 1;
                        }
                }
            }

            if(sig.getMode1D() == Signal.MODE_STEP)
            {
                Vector v = new Vector();
                int x[];
                int y[];
                for(i = 0; i < curr_vect.size(); i++)
                {
                    curr_polygon = (Polygon)curr_vect.elementAt(i);
                    int np = curr_polygon.npoints * 2 - 1;
                    x = new int[np];
                    y = new int[np];
                    for(i = 0, j = 0; i < curr_polygon.npoints; i++, j++)
                    {
                        x[j] = curr_polygon.xpoints[i];
                        y[j] = curr_polygon.ypoints[i];
                        j++;
                        if(j == np)
                            break;
                        x[j] = curr_polygon.xpoints[i+1];
                        y[j] = curr_polygon.ypoints[i];
                    }
                        curr_polygon = new Polygon(x, y, np);
                        v.addElement(curr_polygon);
                }
                curr_vect = v;
            }
            return curr_vect;
    }
    public Vector ToPolygons(Signal sig, Dimension d)
    {
            if(sig.isDoubleX())
              return ToPolygonsDoubleX(sig, d);

            int i, j, curr_num_points, curr_x, start_x, max_points;
            float  max_y, min_y, curr_y;
            Vector curr_vect = new Vector(5);
            int xpoints[], ypoints[];
            Polygon curr_polygon = null;
            int pol_idx = 0;
            min_y = max_y = sig.y[0];
            xpoints = new int[sig.n_points];
            ypoints = new int[sig.n_points];
            //xpoints = new int[2*sig.n_points];
            //ypoints = new int[2*sig.n_points];
            curr_num_points = 0;
            i = j = 0;
        int end_point = sig.n_points;


            if(x_log || y_log)
            {
                double xmax_nolog = Math.pow(10, xmax);
            double xmin_nolog = Math.pow(10, xmin);

            float first_y, last_y;
                for(i = 0; i < sig.n_points && sig.x[i] < xmin_nolog; i++);
                if(i > 0) i--;
                min_y = max_y = sig.y[i];
                j = i+1;
                start_x = XPixel(sig.x[i], d);

                first_y = last_y = sig.y[i];
                while(j < end_point)//sig.n_points  && sig.x[j] < xmax_nolog)
                {
                        for(j = i+1; j < sig.n_points &&
                            (pol_idx >= sig.n_nans || j != sig.nans[pol_idx])&&
                            (curr_x = XPixel(sig.x[j], d)) == start_x; j++)
                        {
                            last_y = curr_y = sig.y[j];
                            if(curr_y < min_y) min_y = curr_y;
                            if(curr_y > max_y) max_y = curr_y;
                        }
                        if(max_y > min_y)
                        {
                            if(first_y != min_y)
                            {
                                xpoints[curr_num_points] = start_x;
                                ypoints[curr_num_points] = YPixel(first_y, d);
                                curr_num_points++;
                            }
                            xpoints[curr_num_points] = xpoints[curr_num_points + 1] = start_x;
                            ypoints[curr_num_points] = YPixel(min_y, d);
                            ypoints[curr_num_points + 1] = YPixel(max_y, d);
                            curr_num_points +=2;
                            if(last_y != max_y)
                            {
                                xpoints[curr_num_points] = start_x;
                                ypoints[curr_num_points] = YPixel(last_y, d);
                                curr_num_points++;
                            }
                        }
                        else
                        {
                            xpoints[curr_num_points] = start_x;
                            ypoints[curr_num_points] = YPixel(max_y, d);
                            curr_num_points++;
                        }
                        if(j == sig.n_points || j == end_point || Float.isNaN(sig.y[j]))// || sig.x[j] >= xmax_nolog)
                        {
                            curr_polygon = new Polygon(xpoints, ypoints, curr_num_points);
                            curr_vect.addElement(curr_polygon);
                            pol_idx++;
                            curr_num_points = 0;
                            if(j < sig.n_points)  //need to raise pen
                            {
                                    while(j < sig.n_points && Float.isNaN(sig.y[j]))
                                        j++;
                            }
                        }
                        if(j < end_point)//sig.n_points)
                        {
                            start_x = XPixel(sig.x[j], d);
                            max_y = min_y = sig.y[j];
                            i = j;
                            if(sig.x[j] > xmax)
                                end_point = j + 1;
                        }
                }
            }
            else // Not using logaritmic scales
            {
                ComputeFactors(d);

                for(i = 0; i < sig.n_points && sig.x[i] < xmin; i++);
                if(i > 0) i--;
                min_y = max_y = sig.y[i];
                j = i+1;

        //GAB testare da qua il problema


                start_x = XPixel(sig.x[i]);
                float first_y, last_y;
                while(j < end_point)//sig.n_points && sig.x[j] < xmax + dt)
                {
                    first_y = last_y = sig.y[i];
                        for(j = i+1; j < sig.n_points && //!Float.isNaN(sig.y[j]) &&
                            (pol_idx >= sig.n_nans || j != sig.nans[pol_idx])&&
                            (curr_x = XPixel(sig.x[j])) == start_x; j++)
                        {
                            last_y = curr_y = sig.y[j];
                            if(curr_y < min_y) min_y = curr_y;
                            if(curr_y > max_y) max_y = curr_y;
                        }
                        if(max_y > min_y)
                        {
                            if(first_y == min_y)
                            {
                                xpoints[curr_num_points] = start_x;
                                ypoints[curr_num_points] = YPixel(first_y);
                                curr_num_points++;
                                if(last_y == max_y)
                                {
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(last_y);
                                    curr_num_points++;
                                }
                                else
                                {
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(max_y);
                                    curr_num_points++;
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(last_y);
                                    curr_num_points++;
                                }
                            }
                            else if(first_y == max_y)
                            {
                                xpoints[curr_num_points] = start_x;
                                ypoints[curr_num_points] = YPixel(first_y);
                                curr_num_points++;
                                if(last_y == min_y)
                                {
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(last_y);
                                    curr_num_points++;
                                }
                                else
                                {
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(min_y);
                                    curr_num_points++;
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(last_y);
                                    curr_num_points++;
                                }
                            }
                            else //first_y != min_y && first_y != max_y
                            {
                                xpoints[curr_num_points] = start_x;
                                ypoints[curr_num_points] = YPixel(first_y);
                                curr_num_points++;
                        if(last_y == min_y)
                        {
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(max_y);
                                    curr_num_points++;
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(last_y);
                                    curr_num_points++;
                                }
                                else if(last_y == max_y)
                                {
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(min_y);
                                    curr_num_points++;
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(last_y);
                                    curr_num_points++;
                                }
                                else
                                {
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(min_y);
                                    curr_num_points++;
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(max_y);
                                    curr_num_points++;
                                    xpoints[curr_num_points] = start_x;
                                    ypoints[curr_num_points] = YPixel(last_y);
                                    curr_num_points++;
                                }
                            }
                        }
                        else
                        {
                            xpoints[curr_num_points] = start_x;
                            ypoints[curr_num_points] = YPixel(max_y);
                            curr_num_points++;
                        }
                        if(j == sig.n_points || j >= end_point || Float.isNaN(sig.y[j]))// || sig.x[j] >= xmax)
                        {
                            curr_polygon = new Polygon(xpoints, ypoints, curr_num_points);
                            curr_vect.addElement(curr_polygon);
                            pol_idx++;
                            curr_num_points = 0;
                            if(j < sig.n_points)  //need to raise pen
                            {
                                    while(j < sig.n_points && Float.isNaN(sig.y[j]))
                                        j++;
                            }
                        }
                        if(j < end_point)//sig.n_points)
                        {
                            start_x = XPixel(sig.x[j]);
                            max_y = min_y = sig.y[j];
                            i = j;
                            if(sig.isIncreasingX() && sig.x[j] > xmax)
                                end_point = j + 1;
                        }
                }
            }

            if(sig.getMode1D() == Signal.MODE_STEP)
            {
                Vector v = new Vector();
                int x[];
                int y[];
                for(i = 0; i < curr_vect.size(); i++)
                {
                    curr_polygon = (Polygon)curr_vect.elementAt(i);
                    int np = curr_polygon.npoints * 2 - 1;
                    x = new int[np];
                    y = new int[np];
                    for(i = 0, j = 0; i < curr_polygon.npoints; i++, j++)
                    {
                        x[j] = curr_polygon.xpoints[i];
                        y[j] = curr_polygon.ypoints[i];
                        j++;
                        if(j == np)
                            break;
                        x[j] = curr_polygon.xpoints[i+1];
                        y[j] = curr_polygon.ypoints[i];
                    }
                        curr_polygon = new Polygon(x, y, np);
                        v.addElement(curr_polygon);
                }
                curr_vect = v;
            }
            return curr_vect;
    }
}
