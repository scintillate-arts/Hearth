#include <iostream>
#include <hearth/hearth.hpp>

int main() {
  // Provide application create info.
  const hearth::Application::CreateInfo appCreateInfo {
    .appName      = L"Hearthfire",
    .appResidency = hearth::Environment::instance(),
    .appVersion   = hearth::Version::current
  };

  // Create application.
  hearth::Application app(appCreateInfo);
  app.run();
}
