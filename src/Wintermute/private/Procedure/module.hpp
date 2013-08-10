/**
 * vim: ft=cpp
 *
 */
namespace Wintermute {
  namespace Procedure {
    class Module;
    class ModulePrivate {
      public:
        Module* q_ptr;
        ModulePrivate (Module* q);
        virtual ~ModulePrivate ();
    };
  } /* Procedure */
} /* Wintermute  */
