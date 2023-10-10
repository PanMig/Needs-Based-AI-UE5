# Needs-Based-AI-UE5

Summary:
The agent simulation is implemented using a utility based AI system called needs-based AI.
Agents have needs (Hunger, fun, energy) and try to perform activities in order to receive
rewards that refill those needs. This is achieved using “smart objects' that advertise their
potential reward and the agent chooses the next activity based on the best score amongst
the advertised rewards. No predefined and hard-coded behaviors are needed, the
needs-based algorithm is capable of always finding the best available activity in the
environment. Designers using data assets can easily add new needs, add smart objects or
change advertisement values without touching cpp or bp code.

Multi agent simulation video:
https://drive.google.com/file/d/1jhmADXJi3JQdrQ3mSpLs85DpZblXxFSX/view?usp=sharing

Decision Making algorithm:
The core decision making process is implemented using the following algorithm in
pseudocode:

![image](https://github.com/PanMig/Needs-Based-AI-UE5/assets/15057375/795b09e6-ee5b-4889-bc43-de474c7574a0)


Scoring system:
![image](https://github.com/PanMig/Needs-Based-AI-UE5/assets/15057375/a557d582-9950-46a3-8d32-d036c370a12a)


Code architecture:
High level UML architecture (some class info are omitted):

![image](https://github.com/PanMig/Needs-Based-AI-UE5/assets/15057375/79ba73ec-9f61-4fd1-8ba4-3385ebb19792)


 
