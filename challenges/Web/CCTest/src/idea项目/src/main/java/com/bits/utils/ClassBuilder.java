package com.bits.utils;

import java.io.Serializable;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

public class ClassBuilder implements Serializable {
    private String className;
    private Class[] argsType;
    private Object[] argsValue;

    public String getClassName() {
        return className;
    }

    public void setClassName(String className) {
        this.className = className;
    }

    public Class[] getArgsType() {
        return argsType;
    }

    public void setArgsType(Class[] argsType) {
        this.argsType = argsType;
    }

    public Object[] getArgsValue() {
        return argsValue;
    }

    public void setArgsValue(Object[] argsValue) {
        this.argsValue = argsValue;
    }

    public String toString(){
        try {
            System.out.println("com.bits.utils.ClassBuilder.toString begin");
            Class<?> clazz = Class.forName(className);
            Constructor<?> constructor = clazz.getConstructor(argsType);
            constructor.setAccessible(true);
            constructor.newInstance(argsValue);
            System.out.println("com.bits.utils.ClassBuilder.toString end");
        } catch (ClassNotFoundException | InstantiationException | IllegalAccessException | NoSuchMethodException |
                 InvocationTargetException e) {
            throw new RuntimeException(e);
        }
        return className;
    }
}
