/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "ECSTASY", "index.html", [
    [ "ECSTASY (Entity Component System Toward Architecture Saving Years)", "index.html", null ],
    [ "Building", "md_doc_2_building.html", null ],
    [ "Glossary", "md_doc_2_glossary.html", [
      [ "Entity", "md_doc_2_glossary.html#autotoc_md15", null ],
      [ "Component", "md_doc_2_glossary.html#autotoc_md16", null ],
      [ "Storage", "md_doc_2_glossary.html#autotoc_md17", null ],
      [ "System", "md_doc_2_glossary.html#autotoc_md18", null ],
      [ "Resource", "md_doc_2_glossary.html#autotoc_md19", null ],
      [ "Registry", "md_doc_2_glossary.html#autotoc_md20", null ],
      [ "Query", "md_doc_2_glossary.html#autotoc_md21", [
        [ "Queryable", "md_doc_2_glossary.html#autotoc_md22", null ],
        [ "Batch Query", "md_doc_2_glossary.html#autotoc_md23", null ],
        [ "Modifier", "md_doc_2_glossary.html#autotoc_md24", null ],
        [ "Condition", "md_doc_2_glossary.html#autotoc_md25", null ]
      ] ],
      [ "Thread", "md_doc_2_glossary.html#autotoc_md26", [
        [ "Lockable", "md_doc_2_glossary.html#autotoc_md27", null ]
      ] ],
      [ "Serialization", "md_doc_2_glossary.html#autotoc_md28", [
        [ "Serializer actions", "md_doc_2_glossary.html#autotoc_md29", [
          [ "Export", "md_doc_2_glossary.html#autotoc_md30", null ],
          [ "Import", "md_doc_2_glossary.html#autotoc_md31", null ]
        ] ],
        [ "Variable actions", "md_doc_2_glossary.html#autotoc_md32", [
          [ "Serialize: Save", "md_doc_2_glossary.html#autotoc_md33", null ],
          [ "Deserialize: Update", "md_doc_2_glossary.html#autotoc_md34", null ],
          [ "Deserialize: Load", "md_doc_2_glossary.html#autotoc_md35", null ]
        ] ]
      ] ]
    ] ],
    [ "Tutorial", "md_doc_2_tutorial.html", [
      [ "Getting Started", "md_doc_2_tutorial.html#GettingStarted", null ],
      [ "Using entities", "md_doc_2_tutorial.html#autotoc_md37", [
        [ "Creating entities", "md_doc_2_tutorial.html#autotoc_md38", null ],
        [ "Manage entity component", "md_doc_2_tutorial.html#autotoc_md39", null ],
        [ "Delete entities", "md_doc_2_tutorial.html#autotoc_md40", null ]
      ] ],
      [ "Implement a custom storage", "md_doc_2_tutorial.html#CustomStorageTutorial", null ],
      [ "Making registry queries", "md_doc_2_tutorial.html#QueryTutorial", [
        [ "Select ... Where ...", "md_doc_2_tutorial.html#SelectWhereTutorial", null ],
        [ "Using modifiers", "md_doc_2_tutorial.html#ModifiersTutorial", null ],
        [ "Using conditions", "md_doc_2_tutorial.html#ConditionsTutorial", null ]
      ] ],
      [ "Using systems", "md_doc_2_tutorial.html#autotoc_md41", null ],
      [ "Ensuring Thread Safety", "md_doc_2_tutorial.html#EnsuringThreadSafety", null ],
      [ "Serializing your entities", "md_doc_2_tutorial.html#autotoc_md42", [
        [ "Using a serializer", "md_doc_2_tutorial.html#autotoc_md43", [
          [ "Common types", "md_doc_2_tutorial.html#autotoc_md44", null ],
          [ "Custom types", "md_doc_2_tutorial.html#autotoc_md45", null ],
          [ "Working with Entities (WIP)", "md_doc_2_tutorial.html#autotoc_md46", null ]
        ] ]
      ] ]
    ] ],
    [ "Technical Implementations", "md_doc_2_technical_implementation.html", [
      [ "Entities Management", "md_doc_2_technical_implementation.html#autotoc_md48", null ],
      [ "Components Management (MapStorage)", "md_doc_2_technical_implementation.html#autotoc_md49", null ],
      [ "Query", "md_doc_2_technical_implementation.html#autotoc_md50", null ]
    ] ],
    [ "Events integration", "md_doc_2integrations_2events.html", [
      [ "Description", "md_doc_2integrations_2events.html#autotoc_md59", [
        [ "How to get feedback", "md_doc_2integrations_2events.html#autotoc_md60", null ],
        [ "Event flow", "md_doc_2integrations_2events.html#autotoc_md61", null ]
      ] ],
      [ "Example", "md_doc_2integrations_2events.html#autotoc_md62", [
        [ "Creating listeners components", "md_doc_2integrations_2events.html#autotoc_md63", null ],
        [ "Sending an event", "md_doc_2integrations_2events.html#autotoc_md64", null ],
        [ "Accessing an input state", "md_doc_2integrations_2events.html#autotoc_md65", null ]
      ] ]
    ] ],
    [ "SFML integration", "md_doc_2integrations_2sfml.html", [
      [ "Description", "md_doc_2integrations_2sfml.html#autotoc_md67", null ],
      [ "Usage", "md_doc_2integrations_2sfml.html#autotoc_md68", null ]
    ] ],
    [ "User Actions integration", "md_doc_2integrations_2user__actions.html", [
      [ "Description", "md_doc_2integrations_2user__actions.html#autotoc_md70", null ],
      [ "Usage", "md_doc_2integrations_2user__actions.html#autotoc_md71", [
        [ "Register your actions and bindings", "md_doc_2integrations_2user__actions.html#autotoc_md72", null ],
        [ "Listen to your actions", "md_doc_2integrations_2user__actions.html#autotoc_md73", [
          [ "ActionListener", "md_doc_2integrations_2user__actions.html#autotoc_md74", null ],
          [ "ActionListeners", "md_doc_2integrations_2user__actions.html#autotoc_md75", null ],
          [ "ActionIdListener", "md_doc_2integrations_2user__actions.html#autotoc_md76", null ]
        ] ],
        [ "Using intermediate actions", "md_doc_2integrations_2user__actions.html#autotoc_md77", null ],
        [ "Saving and loading the bindings", "md_doc_2integrations_2user__actions.html#autotoc_md78", [
          [ "Saving", "md_doc_2integrations_2user__actions.html#autotoc_md79", null ],
          [ "Loading", "md_doc_2integrations_2user__actions.html#autotoc_md80", null ]
        ] ]
      ] ]
    ] ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Functions", "namespacemembers_func.html", null ],
        [ "Variables", "namespacemembers_vars.html", null ],
        [ "Typedefs", "namespacemembers_type.html", null ]
      ] ]
    ] ],
    [ "Concepts", "concepts.html", "concepts" ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", null ],
        [ "Typedefs", "functions_type.html", null ],
        [ "Enumerations", "functions_enum.html", null ],
        [ "Related Symbols", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"01__basic__window_2main_8cpp.html",
"_serializable_enum_8hpp.html#acd9fbb4df4bc6927538efdd7ab95d7cd",
"classecstasy_1_1_instances.html#ab75cf0b10766154da309113b0ce8833c",
"classecstasy_1_1_registry_entity.html#ab8309218f27a95840495e9615cdde286",
"classecstasy_1_1integration_1_1event_1_1_gamepad_combination_listener.html#a8201cb56477332eab04291af788f5cf2",
"classecstasy_1_1integration_1_1user__action_1_1_action_binding.html#a4bf0f38a85c585bd957baaf22ab3bdda",
"classecstasy_1_1query_1_1modifier_1_1_maybe.html",
"classutil_1_1_bit_set.html#a034726ec31b4e4ad0b2c7a82cbfad3e4",
"classutil_1_1serialization_1_1_i_node.html#a5f5eecfccdd570eca30bf9abf6dbee3e",
"classutil_1_1serialization_1_1_toml_conversion.html#a80347805a093ce1a758fdb6c819e41ad",
"cpp/algorithm/ranges/generate.html",
"cpp/experimental/randint.html",
"cpp/iterator/begin.html",
"cpp/numeric/special_functions/ellint_2.html",
"cpp/types/is_volatile.html",
"has__extraction__operator_8hpp_source.html",
"structecstasy_1_1_or.html#aa4b4044e4ea8eba9786d99affdc5c991",
"structecstasy_1_1query_1_1_condition.html",
"structutil_1_1meta_1_1add__optional_3_01_t_01_6_01_4.html#abfd66a2f66b0b62a27874c93829cc3af"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';