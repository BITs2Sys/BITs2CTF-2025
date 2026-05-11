package com.bit.io;

import org.springframework.core.io.ClassPathResource;
import sun.misc.ObjectInputFilter;

import java.io.*;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import java.util.Set;

public class FilteredObjectInputStream extends ObjectInputStream {
    private boolean rootChecked;
    private boolean fullMatch;
    private Class<?>[] whiteList;
    private static Set<String> blackList;

    static {
        ClassPathResource resource = new ClassPathResource("unserializeBlacklist.conf");
        try {
            InputStream inputStream =  resource.getInputStream();
            Properties prop = new Properties();
            prop.load(inputStream);
            Map tem1 = new HashMap(prop);
            blackList = tem1.keySet();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static Set<String> getBlackList() {
        return blackList;
    }

    public FilteredObjectInputStream(InputStream in, Class<?>[] whiteList) throws IOException {
        this(in, whiteList, false);
    }

    public FilteredObjectInputStream(InputStream in, Class<?>[] whiteList, boolean fullMatch) throws IOException {
        super(in);
        this.rootChecked = false;
        this.fullMatch = false;
        this.fullMatch = fullMatch;
        this.whiteList = whiteList;
        ObjectInputFilter.Config.setObjectInputFilter(this, new Filter());
    }

    public Object readNext() throws ClassNotFoundException, IOException {
        this.rootChecked = false;
        return this.readObject();
    }


    private class Filter implements ObjectInputFilter {
        private Filter() {
        }

        public ObjectInputFilter.Status checkInput(ObjectInputFilter.FilterInfo filterinfo) {
            Class<?> clazz = filterinfo.serialClass();
            if (clazz == null) {
                return Status.UNDECIDED;
            } else {
                if (!FilteredObjectInputStream.this.fullMatch) {
                    if (FilteredObjectInputStream.blackList != null && FilteredObjectInputStream.blackList.size() > 0 && FilteredObjectInputStream.blackList.contains(clazz.getName())) {
                        return Status.REJECTED;
                    }

                    if (FilteredObjectInputStream.this.rootChecked) {
                        return Status.ALLOWED;
                    }
                }

                if (FilteredObjectInputStream.this.whiteList != null) {
                    Class[] var3 = FilteredObjectInputStream.this.whiteList;
                    int var4 = var3.length;

                    for(int var5 = 0; var5 < var4; ++var5) {
                        Class white = var3[var5];
                        if (white.isAssignableFrom(clazz)) {
                            FilteredObjectInputStream.this.rootChecked = true;
                            return Status.ALLOWED;
                        }
                    }

                    return Status.REJECTED;
                } else {
                    return Status.ALLOWED;
                }
            }
        }
    }
}
