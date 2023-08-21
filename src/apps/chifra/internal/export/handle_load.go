package exportPkg

//++++++++++++++++++++++++++++++++
// See https://pkg.go.dev/plugin
//++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++
// bool COptions::h andle_traversers(void) {
// CDynamic Traverser lib(load);
// if (!lib.is _valid())
//     return usage("Dynamic library " + load + " was found but is not valid.");
// if (!loadNames())
//     return usage("Could not load names database.");

// auto libFactory = lib.get _function<CTraverser*(uint32_t)>("makeTraverser");
// if (!libFactory) {
//     LOG_ERR("Could not instantiate traverser. Quitting.");
//     return false;
// }
// LOG_INFO(bBlue, "Instantiating traverser", cOff);
// CTraverser* trav = libFactory(getVersionNum());
// if (trav->dataFunc == noopFunc || trav->dataFunc == nullptr)
//     trav->dataFunc = loadTx_Func;
// for (auto monitor : allMonitors) {
//     findName(monitor.address, monitor);
//     trav->monitorMap[monitor.address] = monitor;
// }

// for (auto monitor : allMonitors) {
//     LOG_INFO(bBlue, "Starting traversal of ", monitor.address, cOff);
//     trav->curMonitor = &monitor;
//     curMonitor = &monitor;
//     monitor.read Appearances(visitOnLoad, this);
//     if (reversed)  // TODO: remove this comment once account works backwardly
//         sort(monitor.apps.begin(), monitor.apps.end(), sortMonitoredAppearanceReverse);
//     trav->traverse(monitor.apps, this);
// }
//     return true;
// }
// class CDynamic Traverser {
//   private:
//     void* m _handle;
//     string_q m _path;
//   public:
//     explicit CDynamic Traverser(const string_q& lib _path) {
//         m _handle = ::dlopen(lib _path.c_str(), RTLD_LAZY);
//         m _path = lib _path;
//         if (m _handle == nullptr)
//             LOG_ERR("Failed to load shared object " + lib _path + ": " + string_q(::dlerror()));
//     }
//     ~CDynamic Traverser() {
//         if (m _handle != nullptr) {
//             if (::dl close(m _handle))  // returns non-zero on failure
//                 LOG_ERR("Failed to unload the shared object: " + string_q(::dlerror()));
//         }
//     }
//     template <typename Function Signature>
//     Function Signature* get _function(const string_q& func_name) {
//         void* func_ptr = ::dlsym(m _handle, func_name.c_str());
//         if (func_ptr == nullptr)
//             LOG_ERR("Failed to get [func_name:" + func_name + "]: " + ::dlerror());
//         return reinterpret_cast<Function Signature*>(func_ptr);
//     }
//     bool is _valid(void) const {
//         return m _handle != nullptr;
//     }
//   private:
//     CDynamic Traverser(void) = delete;
//     CDynamic Traverser(const CDynamic Traverser&) = delete;
//     CDynamic Traverser& operator=(const CDynamic Traverser&) = delete;
// };
