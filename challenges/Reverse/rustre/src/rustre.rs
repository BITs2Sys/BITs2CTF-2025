use std::{io::stdin, vec};

fn main() {
    let mut str_buf = String::new();
    stdin().read_line(&mut str_buf).expect("Failed to read line.");
    let input:Vec<u8> = str_buf. trim().as_bytes().to_vec();
    let count = input.clone().into_iter().count();
    if count!=28 {
        println!("Wrong length!");
        return;
    }
    let mut ls:Vec<i32> = vec![];
    let mut i = 0;
    while i < count {
        let a = input[i] as i32;
        ls.push(a);
        i+=1;
    }
    let key:String = "reee".to_string();
    ls=enc(ls, count,key.as_bytes().to_vec());
    let mut output:Vec<i32> = ls.clone();
    i=0;
    while i<count/7 {
        output[7*i]=(ls[7*i]&0x40)|(ls[7*i+1]&0x20)|(ls[7*i+2]&0x10)|(ls[7*i+3]&0x8)|(ls[7*i+4]&0x4)|(ls[7*i+5]&0x2)|(ls[7*i+6]&0x1);
        output[7*i+1]=(ls[7*i]&0x20)|(ls[7*i+1]&0x10)|(ls[7*i+2]&0x8)|(ls[7*i+3]&0x4)|(ls[7*i+4]&0x2)|(ls[7*i+5]&0x1)|(ls[7*i+6]&0x40);
        output[7*i+2]=(ls[7*i]&0x10)|(ls[7*i+1]&0x8)|(ls[7*i+2]&0x4)|(ls[7*i+3]&0x2)|(ls[7*i+4]&0x1)|(ls[7*i+5]&0x40)|(ls[7*i+6]&0x20);
        output[7*i+3]=(ls[7*i]&0x8)|(ls[7*i+1]&0x4)|(ls[7*i+2]&0x2)|(ls[7*i+3]&0x1)|(ls[7*i+4]&0x40)|(ls[7*i+5]&0x20)|(ls[7*i+6]&0x10);
        output[7*i+4]=(ls[7*i]&0x4)|(ls[7*i+1]&0x2)|(ls[7*i+2]&0x1)|(ls[7*i+3]&0x40)|(ls[7*i+4]&0x20)|(ls[7*i+5]&0x10)|(ls[7*i+6]&0x8);
        output[7*i+5]=(ls[7*i]&0x2)|(ls[7*i+1]&0x1)|(ls[7*i+2]&0x40)|(ls[7*i+3]&0x20)|(ls[7*i+4]&0x10)|(ls[7*i+5]&0x8)|(ls[7*i+6]&0x4);
        output[7*i+6]=(ls[7*i]&0x1)|(ls[7*i+1]&0x40)|(ls[7*i+2]&0x20)|(ls[7*i+3]&0x10)|(ls[7*i+4]&0x8)|(ls[7*i+5]&0x4)|(ls[7*i+6]&0x2);
        i+=1;
    }
    output = output.clone().iter().map(|x| (((*x)^square(0b101))>>3)|((((*x)^square(0b101))<<5)%256)).collect();
    let judge:bool = cmp(output);
    if judge {
        println!("You are right!");
    }
    else {
        println!("You are wrong!");
    }
}

fn enc(mut ll:Vec<i32>,n:usize,k:Vec<u8>)->Vec<i32>{
    let mut i: usize = 0;
    let count = k.clone().into_iter().count();
    let mut secret:Vec<i32> = vec![];
    while i<count {
        let a = k[i] as i32;
        secret.push(a);
        i+=1;
    }
    i=0;
    secret=keychange(secret);
    while i < n/8 {
        ll[4*i]^=secret[i%4];
        ll[4*i+1]^=secret[(i+1)%4];
        ll[4*i+2]^=secret[(i+2)%4];
        ll[4*i+3]^=secret[(i+3)%4];
        i+=1;
    }
    return ll;
}

fn keychange(mut k:Vec<i32>)->Vec<i32>{
    let n=4;
    let mut i = 1;
    let mut count=50;
    while i<n {
        if k[i] == k[i-1]{
            k[i]=count;
            count+=1;
        }else {
            count=50;
        }
        i+=1;
    }
    i = 0;
    while i<n-1 {
        k[i]^=k[i+1];
        i+=1;
    }
    return k;
}

fn square(x: i32) -> i32 {
    x * x
}

fn cmp(ll:Vec<i32>)->bool{
    let out:Vec<i32> = vec![139,161,68,233,197,35,129,79,171,76,167,6,37,135,207,236,143,169,41,205,8,197,109,109,205,228,140,128];
    let count = out.clone().into_iter().count();
    for i in 0..count {
        if ll[i]!=out[i]{
            return false;
        }
    }
    return true;
}
//BITs2CTF{}