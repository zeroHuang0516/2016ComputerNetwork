1. 學號：b03901156
2. 姓名：黃于瑄
3. 使用之程式語言：< C >
4. 使用之編譯器：< GNU gcc >
5. 檔案壓縮方式: <zip>
6. 各檔案說明：
	 b03901156_pj1/main/src : source files including Server.cpp, Client.cpp
	 b03901156_pj1/main/bin  : binary files including Server Client 
	 b03901156_pj1/main/Makefile : Makefile
	 b03901156_pj1/main/readme.txt : the file explains how the program executes

7. 編譯方式說明：        	
	 請在main的目錄下，鍵入make all指令，即可完成編譯，
	 在main的目錄下同時產生 Server, Client 兩個執行檔
	 如果要重新編譯，請先執行 make clean 再執行一次 make all
	
8. 執行、使用方式說明：
   主程式：
   編譯完成後，在main目錄下會產生Server, Client兩個執行檔
   執行檔的命令格式為：
	./Server listen_port
	./Client [-n number] [-t timeout] host_1:port_1 host_2:port_2 ...

9. 執行結果說明 ：
	Server:
		output:
		recv from [client_ip:client_port], seq =[seq number]
		ex: recv from 127.0.0.1:65535, seq = 13
	Client:
		output:
		(if server is reachable and RTT is smaller than or equal to timeout)
		recv from [server_ip:server_port], seq = [seq number], RTT = [delay] msec
		ex: recv from 127.0.0.1:80, seq = 0, RTT = 10 msec
		(if server is not reachable or RTT is bigger than timeout)
		timeout when connect to [server_ip:server_port], seq = [seq number]
		ex: timeout when connect to 127.0.0.1:5566, seq = 3
	另外,在Server.cpp及Client.cpp中有額外加入connect/disconnect的訊息以供除錯，但為避免影響output格式，
	將其作為comment
