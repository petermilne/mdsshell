/* $Id: SignalCache.java,v 1.23 2003/08/08 12:35:41 manduchi Exp $ */
import java.security.*;
import java.io.*;


public class SignalCache 
{
 private long min_last_modified = Long.MAX_VALUE;    
 private long max_last_modified = Long.MIN_VALUE;
 private long purge_modified = 0L;
 private long min_cache_size = 0x800000;
 private long max_cache_size = 0x1000000;
 private MessageDigest md;
 private String cache_directory = "";
 private long cache_size = 0L;
  

 SignalCache()
 {
    try
    {
        md = MessageDigest.getInstance("MD5");
        cache_directory = System.getProperty("Signal.cache_directory");
        
        try
        {   
            String s = System.getProperty("Signal.cache_size");
            max_cache_size = Long.parseLong(s);
        } catch (NumberFormatException e){}
        
        if(cache_directory != null)
        {
            File f = new File(cache_directory);
            if(!f.exists() || !f.isDirectory()) 
                f.mkdirs();
            if(f.list() == null)
                cache_directory = null;
            
        }
        
        if(cache_directory == null)
        {
            cache_directory = System.getProperty("user.home") + File.separator + "jScope" + File.separator + "jScopeCache";
            System.getProperties().put("Signal.cache_directory", cache_directory);            
            File f = new File(cache_directory);
            if(!f.exists() || !f.isDirectory()) 
                f.mkdir();
        }
        
        cache_size = initializeCache();
        
    } catch (NoSuchAlgorithmException e)
    {
        System.out.println(""+e);
    }
 }

 public long getCacheSize()
 {
    return cache_size;
 }
 
 public long initializeCache()
 {
    long last_modified;
    long size = 0L;
    File fc;
    File f = new File(cache_directory);
    String list[] = f.list();
    for(int i = 0; i < list.length; i++)
    {
        fc = new File(cache_directory + File.separator + list[i]);
        last_modified = fc.lastModified();
        if(last_modified < min_last_modified)
            min_last_modified = last_modified;
        if(last_modified > max_last_modified)
            max_last_modified = last_modified;
        size += fc.length();
    }
    purge_modified = min_last_modified + (max_last_modified  - min_last_modified)/2;
    return size;
 }
 
 public String cacheFileName(String name)
 {
     byte[] n = name.getBytes();
     byte[] d = md.digest(n);
     StringBuffer out = new StringBuffer(32);
     char ch[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'L', 'M', 'N', 'O', 'P', 'Q', 'R'};
     
     for(int i = 0; i < d.length; i++)
     {
        out.append(ch[(d[i]& 0xF0)>> 4]);
        out.append(ch[(d[i]& 0x0F)]);
     }
     return out.toString();
 }

 public void putCacheData(String provider, String expression, String experiment, long shot, Object data)
 {
 //   if(provider == null || expression == null || data == null) return;
    
    if(expression == null || data == null) return;
    
    if(experiment == null) 
        experiment = "";
        
    String name = //provider.trim().toUpperCase()+
                  expression.trim().toUpperCase()+
                  experiment.trim().toUpperCase()+shot;
    
//    if(isInCache(name)) return;
    
    if(cache_size > max_cache_size)
        purgeCache();
    
    String cache_file = cache_directory + File.separator + cacheFileName(name);
    DataCached cd = new DataCached(expression, experiment, shot, data);
    
    try
    {
        FileOutputStream ostream = new FileOutputStream(cache_file);
        ObjectOutputStream p = new ObjectOutputStream(ostream);

        p.writeObject(cd);
        p.flush();
        ostream.close();
        File fc = new File(cache_file);
        cache_size += fc.length();
        max_last_modified = fc.lastModified();
    } catch (IOException e) {System.out.println(e);}
 }

 public Object getCacheData(String provider, String expression, String experiment, long shot)
 {
    
    
    //if(provider == null || expression == null) return null;
    if(expression == null) return null;

    if(experiment == null) 
        experiment = "";
    
    String name = //provider.trim().toUpperCase()+
                  expression.trim().toUpperCase()+
                  experiment.trim().toUpperCase()+ shot;
    if(!isInCache(name)) return null;
    
    String cache_file = cache_directory + File.separator + cacheFileName(name);
    
    try
    {
        FileInputStream istream = new FileInputStream(cache_file);
        ObjectInputStream p = new ObjectInputStream(istream);
        DataCached dc = null;
        try
        {
            dc = (DataCached)p.readObject();
        } catch (ClassNotFoundException e) {System.out.println(e);}
        istream.close();
        
        if(dc != null && dc.equals(expression, experiment, shot))
            return dc.data;
        
     } catch (IOException e) {System.out.println(e);}
     
     return null;
 }
 
 public void purgeCache()
 {
    String name;
    long modified;
    File fc;
    File f = new File(cache_directory);
    String list[] = f.list();
   
    for(int i = 0; i < list.length; i++)
    {
        name = cache_directory + File.separator + list[i];
        fc = new File(name);
        modified = fc.lastModified();
        if(modified < purge_modified) {
            cache_size -= fc.length();
            fc.delete();
        } 
    }
    min_last_modified = purge_modified;
    purge_modified = min_last_modified + (max_last_modified  - min_last_modified)/2;
 }
 
 public void freeCache()
 {
    String name;
    File fc;
    File f = new File(cache_directory);
    String list[] = f.list();
    for(int i = 0; i < list.length; i++)
    {
        name = cache_directory + File.separator + list[i];
        fc = new File(name);
        fc.delete();
    }
    cache_size = 0;
 }
 
 public boolean isInCache(String name)
 {
    String cache_name = cacheFileName(name);
    File f = new File(cache_directory + File.separator + cache_name);
    return f.exists();
 }
}
