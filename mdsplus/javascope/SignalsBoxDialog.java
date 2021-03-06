/* $Id: SignalsBoxDialog.java,v 1.21 2003/08/08 12:35:41 manduchi Exp $ */
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.io.*;
import javax.swing.*;
import javax.swing.table.*;


public class SignalsBoxDialog extends JDialog
{  
    jScope scope;
    JTable table;
    TableModel dataModel;
            
    SignalsBoxDialog(JFrame f, String title, boolean modal)
    {
        super(f, title, false);
        scope = (jScope)f; 
        dataModel = new AbstractTableModel() 
        {
            public int getColumnCount() { return 2; }
            public int getRowCount() { return WaveInterface.sig_box.signals_name.size();}
            public Object getValueAt(int row, int col) 
            { 
                switch(col)
                {
                    case 0 : return WaveInterface.sig_box.getYexpr(row);
                    case 1 : return WaveInterface.sig_box.getXexpr(row);
                }
                return null;
            }
            public String getColumnName(int col) 
            { 
                switch(col)
                {
                    case 0 : return "Y expression";
                    case 1 : return "X expression";
                }
                return null;
            }
        };

        table = new JTable(dataModel);
        JScrollPane scrollpane = new JScrollPane(table);
        getContentPane().add("Center", scrollpane);
                
        
        JPanel p = new JPanel();
        p.setLayout(new FlowLayout(FlowLayout.CENTER));
    			
	    JButton add = new JButton("Add");
	    add.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e) 
            {
    	        int idx[] = table.getSelectedRows();
    	        String x_expr[] = new String[idx.length];
    	        String y_expr[] = new String[idx.length];
    	        
    	        for(int i = 0; i < idx.length; i++)
    	        {
    	            y_expr[i] = (String)table.getValueAt(idx[i], 0);
    	            x_expr[i] = (String)table.getValueAt(idx[i], 1);
    	        }
    	        scope.wave_panel.AddSignals(null, null, x_expr, y_expr, true, false);
            }
        });
        p.add(add);
/*  
  	    JButton remove = new JButton("Remove");
	    remove.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e) 
            {
    	        int idx[] = table.getSelectedRows();
                table.clearSelection();
    	        
    	        for(int i = 0; i < idx.length; i++)
    	        {
                    WaveInterface.sig_box.removeExpr(idx[i]-i);
    	        }
    	        table.updateUI();
            }
        });
        p.add(remove);
*/
        
        
        JButton cancel = new JButton("Cancel");
	    cancel.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e) 
            {
    	        dispose();	
            }
        });

        p.add(cancel);

        getContentPane().add("South",p);
              
	    pack();	 
	    setLocationRelativeTo(f);
    }
    
}
