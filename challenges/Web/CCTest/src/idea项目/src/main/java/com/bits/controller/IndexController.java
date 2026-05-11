package com.bits.controller;

import com.bits.utils.MyObjectInputStream;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.Base64;

@Controller
public class IndexController {
    @RequestMapping("/deser")
    @ResponseBody
    public String index(@RequestParam(required = false) String data){
        if (data != null && !data.isEmpty()) {
            try{
                byte[] bytes = Base64.getDecoder().decode(data);
                ByteArrayInputStream bais = new ByteArrayInputStream(bytes);
                MyObjectInputStream mois = new MyObjectInputStream(bais);
                mois.readObject();
            }catch (Exception e){
                StringWriter sw = new StringWriter();
                e.printStackTrace(new PrintWriter(sw));
                return sw.toString();
            }
        }else {
            return "please input data";
        }
        return "index";
    }
}
