package com.bit.controller;

import com.bit.io.FilteredObjectInputStream;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.HashMap;

@Controller
public class DeserController {

    @RequestMapping("/deser")
    @ResponseBody
    public String deser(HttpServletRequest request, HttpServletResponse response) throws IOException {
        ObjectInputStream in = null;
        if (request.getParameter("option") != null && request.getParameter("option").equals("deser")) {
            try {
                in = new FilteredObjectInputStream(request.getInputStream(), new Class[]{HashMap.class});
                HashMap<String, String> headInfo = (HashMap) in.readObject();
            } catch (IOException e) {
                StringWriter sw = new StringWriter();
                e.printStackTrace(new PrintWriter(sw));
                return sw.toString();
            } catch (ClassNotFoundException e) {
                StringWriter sw = new StringWriter();
                e.printStackTrace(new PrintWriter(sw));
                return sw.toString();
            }
        }

        return "Let's Deser!";
    }
}
