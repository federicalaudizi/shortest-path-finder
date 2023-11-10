# Shortest Path Finder

## Problem Description
The goal of this project is to plan a route between two service stations (represented as the kilometrical distance from the beginning of the highway) with the minimum number of stops. Each service station has a fleet of electric vehicles with a range for the maximum distance it can travel on a single charge. A new vehicle must be rented at each station where they stop. 
In cases where multiple routes have the same minimum number of stops, the system should chose the route which prioritizes the stations with shorter distances from the start of the highway.

## Input Commands

The input text file contains a sequence of commands, one per line, with the following format. All positive or non-negative integer values can be represented as 32-bit integers.

### 'add-station distance number-of-cars autonomy-car-1 ... autonomy-car-n'

Adds a station at the specified distance, equipped with the given number of cars, each with its autonomy. Expected response: "added" or "not added".

### 'remove-station distance'

Removes the station at the specified distance, if it exists. Expected response: "removed" or "not removed".

### 'add-car station-distance autonomy-car-to-add'

If the station exists, this command adds a car to it. Multiple cars with the same autonomy can be added. Expected response: "added" or "not added".

### 'remove-car station-distance autonomy-car-to-scrap'
Removes a car from the indicated station, if the station exists and has at least one car with the specified autonomy. Expected response: "scraped" or "not scraped".

### 'plan-path starting-station-distance destination-station-distance'
Requests the planning of a route with the constraints mentioned above. Expected response: the stops in order of travel, represented by the distance of the stations from the start of the highway, separated by spaces and followed by a newline at the end. The starting and destination stations must be included; if they coincide, the station is printed only once. If the route does not exist, it prints "no existing path". The planning action does not alter the stations or their vehicle fleets. The given stations will still be present after this command.

## Implementation
The project was implemented using only libc and custom implementations of data structures. In particualr, a sorted array "stations" was implemented, where each element consists of two components: the kilometerical distance of a station v<sub>i</sub> from the highway's starting point and a pointer to a max-heap storing the range of all electric cars parked at station v<sub>j</sub>.
<p align="center">
  <img width="449" alt="Screenshot 2023-11-10 at 13 36 53" src="https://github.com/federicalaudizi/shortest-path-finder/assets/92373780/a792b808-d850-42db-b7a7-dc3afd40cda3">
</p>
  
To find the minimum path in the most efficent way it was implementd a modified version of the Dijkstra algorithm, where:
1. Binary search was used to find the indices of the departure (i) and arrival (j) stations within "stations". (O(n))
2. Three new arrays of lenght i-j were created: prec (used to store the predecessor station), costs (to store the cost to reach a station), and mapping (used to store the distances of stations from the departure station).

