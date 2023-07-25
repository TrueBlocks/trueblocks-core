package exportPkg

//++++++++++++++++++++++++++++++++
// See https://pkg.go.dev/plugin
//++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++
// bool COptions::h andle_traversers(void) {
// CDynamicTraverser lib(load);
// if (!lib.is_valid())
//     return usage("Dynamic library " + load + " was found but is not valid.");
// if (!loadNames())
//     return usage("Could not load names database.");

// auto libFactory = lib.get_function<CTraverser*(uint32_t)>("makeTraverser");
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
//     if (reversed)  // TODO(tjayrush): remove this comment once account works backwardly
//         sort(monitor.apps.begin(), monitor.apps.end(), sortMonitoredAppearanceReverse);
//     trav->traverse(monitor.apps, this);
// }
//     return true;
// }
// class CDynamicTraverser {
//   private:
//     void* m_handle;
//     string_q m_path;
//   public:
//     explicit CDynamicTraverser(const string_q& lib_path) {
//         m_handle = ::dlopen(lib_path.c_str(), RTLD_LAZY);
//         m_path = lib_path;
//         if (m_handle == nullptr)
//             LOG_ERR("Failed to load shared object " + lib_path + ": " + string_q(::dlerror()));
//     }
//     ~CDynamicTraverser() {
//         if (m_handle != nullptr) {
//             if (::dlclose(m_handle))  // returns non-zero on failure
//                 LOG_ERR("Failed to unload the shared object: " + string_q(::dlerror()));
//         }
//     }
//     template <typename FunctionSignature>
//     FunctionSignature* get_function(const string_q& func_name) {
//         void* func_ptr = ::dlsym(m_handle, func_name.c_str());
//         if (func_ptr == nullptr)
//             LOG_ERR("Failed to get [func_name:" + func_name + "]: " + ::dlerror());
//         return reinterpret_cast<FunctionSignature*>(func_ptr);
//     }
//     bool is_valid(void) const {
//         return m_handle != nullptr;
//     }
//   private:
//     CDynamicTraverser(void) = delete;
//     CDynamicTraverser(const CDynamicTraverser&) = delete;
//     CDynamicTraverser& operator=(const CDynamicTraverser&) = delete;
// };
