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
        [ "Creating entities", "md_doc_2_tutorial.html#autotoc_md38", [
          [ "Using type resolution", "md_doc_2_tutorial.html#autotoc_md39", null ]
        ] ],
        [ "Manage entity component", "md_doc_2_tutorial.html#autotoc_md40", null ],
        [ "Delete entities", "md_doc_2_tutorial.html#autotoc_md41", null ]
      ] ],
      [ "Implement a custom storage", "md_doc_2_tutorial.html#CustomStorageTutorial", null ],
      [ "Making registry queries", "md_doc_2_tutorial.html#QueryTutorial", [
        [ "Select ... Where ...", "md_doc_2_tutorial.html#SelectWhereTutorial", null ],
        [ "Using modifiers", "md_doc_2_tutorial.html#ModifiersTutorial", null ],
        [ "Using conditions", "md_doc_2_tutorial.html#ConditionsTutorial", null ]
      ] ],
      [ "Using systems", "md_doc_2_tutorial.html#autotoc_md42", null ],
      [ "Using resources", "md_doc_2_tutorial.html#autotoc_md43", null ],
      [ "Ensuring Thread Safety", "md_doc_2_tutorial.html#EnsuringThreadSafety", null ],
      [ "Serializing your entities", "md_doc_2_tutorial.html#autotoc_md44", [
        [ "Using a serializer", "md_doc_2_tutorial.html#autotoc_md45", [
          [ "Common types", "md_doc_2_tutorial.html#autotoc_md46", null ],
          [ "Custom types", "md_doc_2_tutorial.html#autotoc_md47", null ],
          [ "Working with Entities", "md_doc_2_tutorial.html#autotoc_md48", null ]
        ] ]
      ] ]
    ] ],
    [ "Technical Implementations", "md_doc_2_technical_implementation.html", [
      [ "Entities Management", "md_doc_2_technical_implementation.html#autotoc_md50", null ],
      [ "Components Management (MapStorage)", "md_doc_2_technical_implementation.html#autotoc_md51", null ],
      [ "Query", "md_doc_2_technical_implementation.html#autotoc_md52", null ]
    ] ],
    [ "Events integration", "md_doc_2integrations_2events.html", [
      [ "Description", "md_doc_2integrations_2events.html#autotoc_md61", [
        [ "How to get feedback", "md_doc_2integrations_2events.html#autotoc_md62", null ],
        [ "Event flow", "md_doc_2integrations_2events.html#autotoc_md63", null ]
      ] ],
      [ "Example", "md_doc_2integrations_2events.html#autotoc_md64", [
        [ "Creating listeners components", "md_doc_2integrations_2events.html#autotoc_md65", null ],
        [ "Sending an event", "md_doc_2integrations_2events.html#autotoc_md66", null ],
        [ "Accessing an input state", "md_doc_2integrations_2events.html#autotoc_md67", null ]
      ] ]
    ] ],
    [ "SFML integration", "md_doc_2integrations_2sfml.html", [
      [ "Description", "md_doc_2integrations_2sfml.html#autotoc_md69", null ],
      [ "Usage", "md_doc_2integrations_2sfml.html#autotoc_md70", null ]
    ] ],
    [ "User Actions integration", "md_doc_2integrations_2user__actions.html", [
      [ "Description", "md_doc_2integrations_2user__actions.html#autotoc_md72", null ],
      [ "Usage", "md_doc_2integrations_2user__actions.html#autotoc_md73", [
        [ "Register your actions and bindings", "md_doc_2integrations_2user__actions.html#autotoc_md74", null ],
        [ "Listen to your actions", "md_doc_2integrations_2user__actions.html#autotoc_md75", [
          [ "ActionListener", "md_doc_2integrations_2user__actions.html#autotoc_md76", null ],
          [ "ActionListeners", "md_doc_2integrations_2user__actions.html#autotoc_md77", null ],
          [ "ActionIdListener", "md_doc_2integrations_2user__actions.html#autotoc_md78", null ]
        ] ],
        [ "Using intermediate actions", "md_doc_2integrations_2user__actions.html#autotoc_md79", null ],
        [ "Saving and loading the bindings", "md_doc_2integrations_2user__actions.html#autotoc_md80", [
          [ "Saving", "md_doc_2integrations_2user__actions.html#autotoc_md81", null ],
          [ "Loading", "md_doc_2integrations_2user__actions.html#autotoc_md82", null ]
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
"_storage_concepts_8hpp.html",
"classecstasy_1_1_instances.html#ada76cfbeabe36543abc1cee8ce28e9f1",
"classecstasy_1_1_registry_1_1_select.html#ad2da777410e2aee990c3564a998106ec",
"classecstasy_1_1integration_1_1event_1_1_gamepad_combination_listener.html#a582d5243b93356a9d25447a19bf97095",
"classecstasy_1_1integration_1_1sfml_1_1_render_window.html#a620f8d42347e26a47dc8979fa767e940",
"classecstasy_1_1query_1_1modifier_1_1_binary_modifier.html#a6db978f9a8ed5286409648973fb2d5b3",
"classecstasy_1_1serialization_1_1_raw_serializer.html#a8f93e4286530d22bab9db262aecaa9d8",
"classutil_1_1_bit_set_1_1_reference.html#af77f2857c5c505b333c619de59446340",
"classutil_1_1serialization_1_1_i_node_factory.html#ab5da91311c808c8c4a333a3b8211f4ca",
"classutil_1_1serialization_1_1_toml_object_node.html#a86e49accbb138e5c70696a1f6eec9a6d",
"cpp/algorithm/shift.html",
"cpp/experimental/type_trait_variable_templates.html",
"cpp/memory/shared_ptr/pointer_cast.html",
"cpp/string/byte/strtof.html",
"dir_13af086a2b4f3f42e1124b964311462f.html",
"md_doc_2integrations_2sfml.html#autotoc_md70",
"structecstasy_1_1integration_1_1event_1_1_event.html#a428f3f906bf2f40cf2dcec224662719aa8ed4d3c0783d6f909907e1592ffe47ac",
"structecstasy_1_1query_1_1_select_1_1_filter_queryables.html#a9bd5caf2dbefc4a90f87d15d9b5d3b8e"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';