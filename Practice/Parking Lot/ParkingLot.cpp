#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

enum class VehicleType
{
    BIKE,
    CAR,
    TRUCK
};

class Vehicle
{
private:
    string licensePlate;
    string ownerName;
    VehicleType type;

public:
    Vehicle(string plate, string name, VehicleType vType)
    {
        licensePlate = plate;
        ownerName = name;
        type = vType;
    }
    string getLicensePlate()
    {
        return licensePlate;
    }
    string getOwnerName()
    {
        return ownerName;
    }
    VehicleType getVehicleType()
    {
        return type;
    }
};

enum class SpotType
{
    BIKE_SPOT,
    CAR_SPOT,
    TRUCK_SPOT
};

class ParkingSpot
{
private:
    int spotId;
    SpotType type;
    bool isAvailable;
    Vehicle *parkedVehicle;

public:
    ParkingSpot(int id, SpotType spotType)
    {
        spotId = id;
        type = spotType;
        isAvailable = true;
        parkedVehicle = nullptr;
    }
    int getSpotId()
    {
        return spotId;
    }
    SpotType getSpotType()
    {
        return type;
    }
    bool getAvailability()
    {
        return isAvailable;
    }
    Vehicle *getParkedVehicle()
    {
        return parkedVehicle;
    }
    bool canParkVehicle(Vehicle *vehicle)
    {
        if (isAvailable)
        {
            if ((type == SpotType::BIKE_SPOT && vehicle->getVehicleType() == VehicleType::BIKE) ||
                (type == SpotType::CAR_SPOT && vehicle->getVehicleType() == VehicleType::CAR) ||
                (type == SpotType::TRUCK_SPOT && vehicle->getVehicleType() == VehicleType::TRUCK))
            {
                return true;
            }
        }
        return false;
    }
    bool assignVehicle(Vehicle *vehicle)
    {
        if (canParkVehicle(vehicle))
        {
            parkedVehicle = vehicle;
            isAvailable = false;
            return true;
        }
        return false;
    }
    void removeVehicle()
    {
        parkedVehicle = nullptr;
        isAvailable = true;
    }
};

// ParkingLotManager::assignVehicle()
// and manager directly modifies:
// spot->isAvailable = false;
// spot->parkedVehicle = vehicle;
// Bad.
// Now:
// ParkingSpot state is managed outside ParkingSpot.
// Violation of encapsulation.

class Ticket
{
private:
    int ticketId;
    Vehicle *vehicle;
    ParkingSpot *spot;
    time_t entryTime;
    time_t exitTime;
    bool isActive;

public:
    Ticket(int id, Vehicle *v, ParkingSpot *s)
    {
        ticketId = id;
        vehicle = v;
        spot = s;
        entryTime = time(0);
        exitTime = 0;
        isActive = true;
    }
    int getTicketId()
    {
        return ticketId;
    }
    Vehicle *getVehicle()
    {
        return vehicle;
    }
    ParkingSpot *getParkingSpot()
    {
        return spot;
    }
    time_t getEntryTime()
    {
        return entryTime;
    }
    time_t getExitTime()
    {
        return exitTime;
    }
    bool getIsActive()
    {
        return isActive;
    }
    void closeTicket()
    {
        exitTime = time(0);
        isActive = false;
    }
};

class Charges
{
private:
    double amount;
    double ratePerHour;
    long duration;

public:
    Charges(double rate)
    {
        ratePerHour = rate;
        amount = 0;
        duration = 0;
    }
    double calculateCharges(time_t entryTime, time_t exitTime)
    {
        duration = difftime(exitTime, entryTime) / 3600; // Convert seconds to hours
        amount = duration * ratePerHour;
        return amount;
    }
    double getAmount()
    {
        return amount;
    }
    long getDuration()
    {
        return duration;
    }
};

class ParkingLot
{
private:
    int lotId;
    string name;
    string location;
    vector<ParkingSpot *> spots;

public:
    ParkingLot(int id, string n, string loc)
    {
        lotId = id;
        name = n;
        location = loc;
    }
    void addParkingSpot(ParkingSpot *spot)
    {
        spots.push_back(spot);
    }
    vector<ParkingSpot *> getAvailableSpots(VehicleType vType)
    {
        vector<ParkingSpot *> availableSpots;
        for (auto spot : spots)
        {
            if (spot->getAvailability() &&
                ((vType == VehicleType::BIKE && spot->getSpotType() == SpotType::BIKE_SPOT) ||
                 (vType == VehicleType::CAR && spot->getSpotType() == SpotType::CAR_SPOT) ||
                 (vType == VehicleType::TRUCK && spot->getSpotType() == SpotType::TRUCK_SPOT)))
            {
                availableSpots.push_back(spot);
            }
        }
        return availableSpots;
    }
};

class ParkingLotManager
{
private:
    ParkingLot *parkingLot;

public:
    ParkingLotManager(ParkingLot *lot)
    {
        parkingLot = lot;
    }

    Ticket *assignVehicleToSpot(Vehicle *vehicle)
    {

        vector<ParkingSpot *> availableSpots = parkingLot->getAvailableSpots(vehicle->getVehicleType());

        if (availableSpots.empty())
        {
            cout << "No available parking spot for this vehicle type." << endl;
            return nullptr;
        }
        ParkingSpot *spot = availableSpots[0];
        if (spot->assignVehicle(vehicle))
        {
            static int ticketIdCounter = 1;
            Ticket *ticket = new Ticket(ticketIdCounter++, vehicle, spot);
            cout << "Vehicle parked successfully. " << "Ticket ID: " << ticket->getTicketId() << endl;
            return ticket;
        }
        else
        {
            cout << "Failed to assign vehicle to parking spot." << endl;
            return nullptr;
        }
    }
    void releaseSpot(Ticket *ticket)
    {
        if (ticket == nullptr)
        {
            cout << "Invalid ticket." << endl;
            return;
        }
        if (!ticket->getIsActive())
        {
            cout << "Ticket already closed." << endl;
            return;
        }
        ParkingSpot *spot = ticket->getParkingSpot();
        spot->removeVehicle();
        ticket->closeTicket();
        cout << "Parking spot released successfully." << endl;
    }
};
int main()
{
    ParkingLot lot(
        1,
        "Phoenix Mall Parking",
        "Pune"
    );

    // Only ONE spot of each type
    lot.addParkingSpot(
        new ParkingSpot(
            1,
            SpotType::CAR_SPOT
        )
    );

    lot.addParkingSpot(
        new ParkingSpot(
            2,
            SpotType::BIKE_SPOT
        )
    );

    lot.addParkingSpot(
        new ParkingSpot(
            3,
            SpotType::TRUCK_SPOT
        )
    );

    ParkingLotManager manager(&lot);

    Vehicle* car1 =
        new Vehicle(
            "MH12AB1234",
            "Akshat",
            VehicleType::CAR
        );

    Vehicle* car2 =
        new Vehicle(
            "MH14CD5678",
            "Rahul",
            VehicleType::CAR
        );

    Vehicle* bike1 =
        new Vehicle(
            "MH01XY1111",
            "Aman",
            VehicleType::BIKE
        );

    Vehicle* truck1 =
        new Vehicle(
            "MH99TR9999",
            "Logistics Ltd",
            VehicleType::TRUCK
        );

    cout << "\n===== TEST 1 : Park First Car =====\n";

    Ticket* carTicket1 =
        manager.assignVehicleToSpot(car1);

    cout << "\n===== TEST 2 : Park Second Car =====\n";

    Ticket* carTicket2 =
        manager.assignVehicleToSpot(car2);

    // Should fail because only one CAR spot exists

    cout << "\n===== TEST 3 : Park Bike =====\n";

    Ticket* bikeTicket =
        manager.assignVehicleToSpot(bike1);

    cout << "\n===== TEST 4 : Park Truck =====\n";

    Ticket* truckTicket =
        manager.assignVehicleToSpot(truck1);

    cout << "\n===== TEST 5 : Release First Car =====\n";

    manager.releaseSpot(carTicket1);

    cout << "\n===== TEST 6 : Park Second Car Again =====\n";

    carTicket2 =
        manager.assignVehicleToSpot(car2);

    // Should succeed now

    cout << "\n===== TEST 7 : Release Same Ticket Twice =====\n";

    manager.releaseSpot(carTicket1);

    cout<< "\n===== TEST 8 : Release Car2 Ticket =====\n";
    manager.releaseSpot(carTicket2);

    cout << "\n===== TEST 9 : Null Ticket =====\n";

    manager.releaseSpot(nullptr);

    cout << "\n===== TEST 10 : Force Car into Bike Spot =====\n";

    ParkingSpot* bikeSpot =
        new ParkingSpot(
            100,
            SpotType::BIKE_SPOT
        );

    bool success =
        bikeSpot->assignVehicle(car1);

    if(!success)
    {
        cout
            << "Car cannot be parked in Bike Spot."
            << endl;
    }

    return 0;
}