This project proposes an intelligent Internet of Things (IoT) network to optimize power consumption in large
halls, amphitheater, auditoriums, theatres, dining halls, etc., by controlling the appliances based on motion detection. The
proposed network deploys individual sensor nodes at different zones with a microwave radar module for human presence detection and monitoring. 
The RCWL radar module, utilizing the Doppler effect and operating at 3.18 GHz, is used to sense objects in confined aperture-based zones. 
After sensing human motion, the node utilizes the NodeMCU microcontroller board to switch the relay using an intelligent switching algorithm.
Using the MQTT protocol, data is collected from each sensor node in the network and is shared and stored in a secure database. Node-RED dashboard is 
used to monitor and control the network remotely. An accuracy of nearly 80% is achieved after multiple iterations of prototyping and measurements using
a 7.5-digit Keithley multimeter, RSA306B spectrum analyzer, and EMI and EMC probes.
