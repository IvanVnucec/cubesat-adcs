function drawPaperPlane(qib=[1 0 0 0])

    % Tocke zrakoplova u koordinatnom sustavu tijela
    P1 = [-1,  1, 0]';
    P2 = [-1, -1, 0]';
    P3 = [ 2,  0, 0]';
    
    P4 = [-1,  0.25,    0]';
    P5 = [-1, -0.25,    0]';
    P6 = [-1,     0, -0.4]';

    % Skaliranje i spremanje tocaka tijela u stupce matrice Pb
    Pb = 0.5*[P1, P2, P3, P4, P5, P6];

    % ---------------------------------------------------------------------
    % Trazenje tocaka u inercijalnom koordinatnom sustavu
    % ---------------------------------------------------------------------
    
    % Iz kvaterniona racunam rotacijsku matricu I->B
    Rib = qib2Rib(qib);
    
    % Izracun rotacijske matrice B->I (obicna transpozicija)
    Rbi = Rib';
    
    % Trazenje reprezentacije pojedine tocke zrakoplova u inercijalnom
    % sustavu i spremanje njezinih koordinata u stupce matrice Pi
    Pi = Rbi*Pb;
    
    % Brisanje prikaza na slici
    %clf;
    
    % Crtanje koordinatnih osi inercijalnog sustava
    hold on;
    plot3(100*[-1,1],[0,0],[0,0],'Color','red','Linewidth',0.75);
    plot3([0,0],100*[-1,1],[0,0],'Color','green','Linewidth',0.75);
    plot3([0,0],[0,0],100*[-1,1],'Color','blue','Linewidth',0.75);
    plot3([0,0],[0,0],100*[-1,1],'Color','blue','Linewidth',0.75);
    
    % Crtanje baznih vektora koordinatnog sustava tijela
    arrow3d([0,Rbi(1,1)],[0,Rbi(2,1)],[0,Rbi(3,1)],0.8,0.02,0.04,'red');
    arrow3d([0,Rbi(1,2)],[0,Rbi(2,2)],[0,Rbi(3,2)],0.8,0.02,0.04,'green');
    arrow3d([0,Rbi(1,3)],[0,Rbi(2,3)],[0,Rbi(3,3)],0.8,0.02,0.04,'blue');
    
    % Crtanje stranica tijela
    P1i = Pi(:,1);
    P2i = Pi(:,2);
    P3i = Pi(:,3);
    P4i = Pi(:,4);
    P5i = Pi(:,5);
    P6i = Pi(:,6);
    
    line3d(P1i, P4i, '-', 'color', 'black');
    line3d(P2i, P3i, '-', 'color', 'black');
    line3d(P1i, P3i, '-', 'color', 'black');
    line3d(P5i, P2i, '-', 'color', 'black');
    line3d(P4i, P3i, '-', 'color', 'black');
    line3d(P5i, P3i, '-', 'color', 'black');
    line3d(P4i, P6i, '-', 'color', 'black');
    line3d(P5i, P6i, '-', 'color', 'black');
    line3d(P6i, P3i, '-', 'color', 'black');
    
    axis equal;
    grid on;
    xlim(2*[-1,1]);
    ylim(2*[-1,1]);
    zlim(2*[-1,1]);
    
    xlabel('x');
    ylabel('y');
    zlabel('z');

    view(3);
end

