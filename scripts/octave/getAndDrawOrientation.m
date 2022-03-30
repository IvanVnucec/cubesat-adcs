function getAndDrawOrientation(serial_port="")
  pkg load instrument-control;
  
  s1 = serial(serial_port, 115200);
  srl_flush(s1);
  
  while true
    % read just one char in blocking mode
    data = srl_read(s1, 1);
    char_data = char(data);
    
    % TODO: add parsing of string to quaternion here
    q = [1 0 0 0];
    pause(0.5);
    drawPaperPlane(q);
  end
  
  fclose(s1);
