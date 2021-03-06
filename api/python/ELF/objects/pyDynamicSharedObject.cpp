/* Copyright 2017 - 2021 R. Thomas
 * Copyright 2017 - 2021 Quarkslab
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "pyELF.hpp"

#include "LIEF/ELF/hash.hpp"

#include "LIEF/ELF/DynamicSharedObject.hpp"
#include "LIEF/ELF/DynamicEntry.hpp"

#include <string>
#include <sstream>

namespace LIEF {
namespace ELF {

template<class T>
using getter_t = T (DynamicSharedObject::*)(void) const;

template<class T>
using setter_t = void (DynamicSharedObject::*)(T);


template<>
void create<DynamicSharedObject>(py::module& m) {

  //
  // Dynamic Shared Object object
  //
  py::class_<DynamicSharedObject, DynamicEntry>(m, "DynamicSharedObject")
    .def(py::init<const std::string &>(),
        "Constructor from library name",
        "library_name"_a)

    .def_property("name",
        [] (const DynamicSharedObject& obj) {
          return safe_string_converter(obj.name());
        },
        static_cast<setter_t<const std::string&>>(&DynamicSharedObject::name),
        "Return the library name")

    .def("__eq__", &DynamicSharedObject::operator==)
    .def("__ne__", &DynamicSharedObject::operator!=)
    .def("__hash__",
        [] (const DynamicSharedObject& entry) {
          return Hash::hash(entry);
        })

    .def("__str__",
        [] (const DynamicSharedObject& dynamicSharedObject)
        {
        std::ostringstream stream;
        stream << dynamicSharedObject;
        std::string str =  stream.str();
        return str;
        });
}

}
}
