package com.bits.utils;

import org.apache.commons.collections.Transformer;

import java.io.*;
import java.net.URLClassLoader;
import java.util.ArrayList;

public class MyObjectInputStream extends ObjectInputStream {
    private String[] denyClasses;

    public MyObjectInputStream(ByteArrayInputStream bais) throws IOException {
        super(bais);
        ArrayList<String> classList = new ArrayList<>();
        InputStream file = MyObjectInputStream.class.getResourceAsStream("/blacklist.txt");
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(file));
        while (true) {
            String denyClass = bufferedReader.readLine();
            if (denyClass != null) {
                classList.add(denyClass.trim());
            } else {
                this.denyClasses = new String[classList.size()];
                classList.toArray(this.denyClasses);
                return;
            }
        }
    }

    @Override // java.io.ObjectInputStream
    protected Class<?> resolveClass(ObjectStreamClass desc) throws IOException, ClassNotFoundException {
        String className = desc.getName();
        for (String denyClass : denyClasses) {
            if (className.startsWith(denyClass)) {
                throw new InvalidClassException("Unauthorized deserialization attempt", className);
            }
        }
        return super.resolveClass(desc);
    }
}
