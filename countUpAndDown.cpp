#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx; // Mutex to protect shared data
std::condition_variable cv; // Condition variable to synchronize threads
bool ready = false; // Flag that shows when the first thread has finished counting up

// Function to count up from 0 to 20
void countUp() {
	for (int i = 0; i <= 20; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Wait 100 milliseconds between each execution
		std::lock_guard<std::mutex> lock(mtx); // Lock mutex to safely print
		std::cout << "Count Up: " << i << std::endl;
	}
	{
		std::lock_guard<std::mutex> lock(mtx);
		ready = true; // Set the flag to true when counting is done
	}
	cv.notify_one(); // Notifies other thread that it can start counting
}

// Function to count down from 20 to 0
void countDown() {
	std::unique_lock<std::mutex> lock(mtx); // Lock mutex to wait for the condition variable
	cv.wait(lock, []() -> bool {return ready; }); // Wait until the 'read' flag is true
	
	for (int i = 20; i >= 0; --i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work by sleeping for 100 milliseconds
		std::cout << "Count Down: " << i << std::endl;
	}
}

int main() {
	std::thread thread1(countUp); // Create the first thread to count up
	std::thread thread2(countDown); // Create the second thread to count down
	
	// Wait for both threads to finish
	thread1.join();
	thread2.join();
	
	return 0;
}