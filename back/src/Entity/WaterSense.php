<?php

namespace App\Entity;

use ApiPlatform\Metadata\ApiResource;
use App\Repository\WaterSenseRepository;
use Doctrine\ORM\Mapping as ORM;

#[ORM\Entity(repositoryClass: WaterSenseRepository::class)]
#[ApiResource]
class WaterSense
{
    #[ORM\Id]
    #[ORM\GeneratedValue]
    #[ORM\Column]
    private ?int $id = null;

    #[ORM\Column(nullable: true)]
    private ?int $water_level = null;

    #[ORM\ManyToOne(inversedBy: 'waterSenses')]
    private ?User $User = null;

    public function getId(): ?int
    {
        return $this->id;
    }

    public function getWaterLevel(): ?int
    {
        return $this->water_level;
    }

    public function setWaterLevel(?int $water_level): static
    {
        $this->water_level = $water_level;

        return $this;
    }

    public function getUser(): ?User
    {
        return $this->User;
    }

    public function setUser(?User $User): static
    {
        $this->User = $User;

        return $this;
    }
}
