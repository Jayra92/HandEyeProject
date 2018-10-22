%Based off the RME40003_example.m file

%% Defining the joints
% These are the definitions of all joints with offsets drawn from the
%SCORBOT-ER9 manual
Base = RevoluteMDH('d', 3.88, 'a', 0, 'alpha', 0);
Shoulder = RevoluteMDH('d', 0.7, 'a', 0.83, 'alpha', pi/2);
Elbow = RevoluteMDH('d', 0, 'a', 2.8, 'alpha', 0);
Wrist_Pitch = RevoluteMDH('d', -0.4, 'a', 2.3, 'alpha', 0);
Wrist_Roll = RevoluteMDH('d', 0, 'a', 1.11, 'alpha', 0);

%% Set up the manipulator
% Create a robot object from the joints defined above
scorbot = SerialLink([Base Shoulder Elbow Wrist_Pitch Wrist_Roll], 'name', 'Scorbot ER9');

%% Defining the positions
% Sets the preferred joint angles for each position. The final is zero as
% the rotation of the wrist join has no bearing on the simulation
qs = [0, -44.83*pi/180, 52.82*pi/180, 70*pi/180, 0]; 
q1 = [-30*pi/180, -10.36*pi/180, -67.62*pi/180, 53.81*pi/180, 0];
q2 = [-45*pi/180, -44.83*pi/180, 52.82*pi/180, 70*pi/180, 0];
qe = [-60*pi/180, -10.36*pi/180, -67.62*pi/180, 53.81*pi/180, 0];

%% Creating the equations
% These define the forward kinematic equations for each position and then
% the inverse kinematic equations to move from one position to the next.
% The final inverse kinematic equation will return the robot to it's
% starting position
fk_qs = scorbot.fkine(qs);
fk_q1 = scorbot.fkine(q1);
fk_q2 = scorbot.fkine(q2);
fk_qe = scorbot.fkine(qe);

ik_qs = scorbot.ikine(fk_qs, q1, 'mask',[1 1 1 0 0 0], 'ilimit', 5000);
ik_q1 = scorbot.ikine(fk_q1, q2, 'mask',[1 1 1 0 0 0], 'ilimit', 5000);
ik_q2 = scorbot.ikine(fk_q2, qe, 'mask',[1 1 1 0 0 0], 'ilimit', 5000);
ik_qe = scorbot.ikine(fk_qe, qs, 'mask',[1 1 1 0 0 0], 'ilimit', 5000);

%% Preparing the animation
% Creates a series of 10 points along each equation that the robot will
% move through to reach a position
num_points = 10;

js = jtraj(ik_qs,ik_q1,num_points);
j1 = jtraj(ik_q1,ik_q2,num_points);
j2 = jtraj(ik_q2,ik_qe,num_points);
je = jtraj(ik_qe,ik_qs,num_points);

scorbot.plotopt = {'workspace',[-1, 10, -10, 1, -1, 10],'scale', 1, 'reach',1, 'delay', 0.5};

% Plot the animation
scorbot.plot(js)
scorbot.plot(j1)
scorbot.plot(j2)
scorbot.plot(je)