# rawToAnyFormat
raw 转换成其他格式

//demo  
RawToAnyForm *anyForm = new RawToAnyForm;  
char finename[256] = "green.raw";  
cv::Mat dst;  
anyForm->convertForm(finename, dst, 2448, 2048, YUV422_YUYV_Packed);
