namespace util {
  /*
* Taken from https://www.tutorialspoint.com/cplusplus-equivalent-of-instanceof
*/
  template<typename Base, typename T>
  inline bool instance_of(const T* ptr) {
    return dynamic_cast<const Base*>(ptr) != nullptr;
  }
}
