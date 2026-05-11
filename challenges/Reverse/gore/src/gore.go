package main

import(
	"fmt"
	"math/rand"
	"strconv"
)

func keyexpand(box *[]int, k *[]int, length int){
	var j,l int
	l=256
	j=0
	for i:=0; i<l; i++ {
		(*box)[i]=i
	}
	for i:=0; i<l;i++ {
		j=(j+(*box)[i]+(*k)[i%length])%256
		(*box)[i]^=(*box)[j]
		(*box)[j]^=(*box)[i]
		(*box)[i]^=(*box)[j]
	}
}

func crypt(box *[]int, data *[]int,ran *[]int, length int){
	var i,j,t,l int = 0,0,0,256
	for k:=0; k<length;k++ {
		i=(i+1)%l
		j=(j+(*box)[i])%l
		(*box)[i]^=(*box)[j]
		(*box)[j]^=(*box)[i]
		(*box)[i]^=(*box)[j]
		t=(((*box)[i]+(*box)[j])%l)^(*ran)[(i+j)%length]
		(*data)[k]^=(*box)[t]
	}
}

func cmp(res *[]int, length int){
	var cmpls = []int{190,103,243,118,225,92,160,121,31,66,167,2,230,153,141,227,206,16,48,54,246,28,27,161,77,37,69,67,238,53,252,177,135,179,187,93,38,30,231,8,80,252,78,164,126,69,230,252,201,24,56,16,166,75,127,213}
	if length!=len(cmpls){
		fmt.Println("You are wrong!")
		return
	}
	for i:=0;i<length;i++{
		if (*res)[i]!=cmpls[i] {
			fmt.Println("You are wrong!")
			return
		}
	}
	fmt.Println("You are right!")
	return
}

func main(){
	var flag string
	fmt.Print("Input your flag:")
	fmt.Scanf("%s", &flag)
	if len(flag)<10{
		fmt.Print("Wrong length!")
		return
	}
	rand.Seed(int64(len(flag)))
	ls := make([]int, (len(flag)-10)*2)
	r := make([]int, (len(flag)-10)*2)
	s := make([]int, 256)
	key := make([]int, 10)
	for index, val := range flag{
		if index < 9{
			key[index] = int(val)
		}else if index == len(flag)-1{
			key[9]=int(val)
		}else{
			ls[2*(index-9)] = int(strconv.FormatInt(int64(val), 16)[1])
			ls[2*(index-9)+1] = int(strconv.FormatInt(int64(val), 16)[0])
			r[2*(index-9)+1] = rand.Intn(255)
			r[2*(index-9)] = rand.Intn(255)
		}
	}
	for i:=0;i<len(ls);i++{
		ls[i]^=ls[(i+1)%len(ls)]
	}
	keyexpand(&s, &key, len(key))
	crypt(&s, &ls, &r, len(ls))
	cmp(&ls, len(ls))
}
