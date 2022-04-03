function getAndDrawOrientation(serial_port="COM7")
  pkg load instrument-control;
  
  s1 = serial(serial_port, 115200);
  srl_flush(s1);
  
  q_str = char(zeros(1, 100));
  while true
    srl_flush(s1);
    % read just one char in blocking mode
    data = srl_read(s1, 1);
    
    % skip until '[' char
    if data != 0x5B
      continue;
    endif
    
    % save everything until ']' char
    i = 1;
    while true
      data = srl_read(s1, 1);
      if data == 0x5D
        break;
      endif
    
      q_str(i) = char(data);
      i++;
    endwhile
        
    printf("%s\n", q_str);
    q = str2num(q_str(1:i-1));
    drawPaperPlane(q);
    
    % wait for plot
    drawnow;
  endwhile
  
    fclose(s1);
